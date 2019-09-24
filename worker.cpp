#include "worker.h"

#include <fstream>
#include <endian.h>
#include <vector>
#include <iterator>
#include <algorithm>  // std::transform
#include <numeric>    // std::accumulate

// Find last set bit
// Returns the position of the last (most significant) bit set in i. The least
//   significant bit is position 1. This function returns 0 if no bits are set
//   in i.
// Copied (with minor stilistic adaptations) from the freeBSD
//   implementation at:
//   https://github.com/freebsd/freebsd/blob/master/sys/libkern/fls.c
static int fls(int i);

void do_work(
  int thread_id,
  std::vector<BlockingQueue>& output_queue,
  WorkerContext& ctx)
{
  if (ctx.input.peek() == EOF) {
    output_queue[thread_id].close();
    return;
  }

  // collect samples
  std::vector<uint32_t> samples;
  for (int i = 0; i < ctx.N; i++) {
    uint32_t s;
    ctx.input.read((char*) &s, sizeof(s));
    samples.push_back(s);
  }

  // change to little endian
  std::transform(samples.begin(), samples.end(), samples.begin(),
    [](uint32_t num){ return be32toh(num); });

  // find reference point (minimum of all samples)
  const uint32_t reference = std::accumulate(samples.begin(), samples.end(),
    samples[0], [](int a, int b){ return std::min(a, b); });

  // normalize
  std::transform(samples.begin(), samples.end(), samples.begin(),
    [reference](uint32_t num){ return num - reference; });

  // find length in bits
  std::vector<int> bit_lengths(samples.size());
  std::transform(samples.begin(), samples.end(), bit_lengths.begin(),
    [](uint32_t num){ return fls(num); });

  // find max length
  const uint32_t max_bits = std::accumulate(bit_lengths.begin(),
    bit_lengths.end(), 0, [](int a, int b){ return std::max(a, b); });

  // create record
  Record r(max_bits, ctx.N);
  r.reference = htobe32(reference);

  // load samples
  for (auto s : samples) r.push_sample(s);

  // push record into queue
  output_queue[thread_id].push(std::move(r));
  output_queue[thread_id].close();
}

static int fls(int i) {
  if (!i) return 0;

  int bit;
  for (bit = 1; i != 1; bit++) i >>= 1;
  return bit;
}

BlockingQueue::BlockingQueue(int size)
  : mtx(new std::mutex),
  cv(new std::condition_variable),
  q(),
  max_size(size),
  closed(false)
{}

BlockingQueue::BlockingQueue(BlockingQueue&& other)
  : mtx(other.mtx),
    cv(other.cv),
    q(std::move(other.q)),
    max_size(other.max_size),
    closed(other.closed)
{
  other.mtx = nullptr;
  other.cv = nullptr;
}

BlockingQueue& BlockingQueue::operator=(BlockingQueue&& other) {
  this->mtx = other.mtx;
  other.mtx = nullptr;
  this->cv = other.cv;
  other.cv = nullptr;
  this->q = std::move(other.q);
  this->max_size = other.max_size;
  return *this;
}

BlockingQueue::~BlockingQueue() {
  if (this->mtx) delete(this->mtx);
  if (this->cv) delete(this->cv);
}

void BlockingQueue::push(Record&& x) {
  std::unique_lock<std::mutex> lock(*mtx);
  q.push(std::move(x));
  cv->notify_one();
}

Record BlockingQueue::pop() {
  std::unique_lock<std::mutex> lock(*mtx);
  while (q.size() == 0 && !closed)
    cv->wait(lock);

  Record result = std::move(q.front());
  q.pop();
  cv->notify_one();
  return result;
}

void BlockingQueue::close() {
  std::unique_lock<std::mutex> lock(*mtx);
  closed = true;
  cv->notify_one();
}

bool BlockingQueue::isClosed() {
  std::unique_lock<std::mutex> lock(*mtx);
  cv->notify_one();
  return (q.size() == 0) && closed;
}

void Record::push_sample(uint32_t s) {
  // find current writing position
  char *p = samples + used_bits / 8;

  s <<= 8 * sizeof(s) - sample_size_bits; // significant bits at begining
  s >>= used_bits % 8;                    // align to last written bit

  while (s) {
    const unsigned char high_byte = s >> (8 * sizeof(s) - 8);
    *(p++) |= high_byte;
    s <<= 8;
  }
  used_bits += sample_size_bits;
}
