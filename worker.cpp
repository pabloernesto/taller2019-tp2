#include "worker.h"

#include <fstream>
#include <endian.h>
#include <vector>

void do_work(
  int thread_id,
  std::vector<BlockingQueue>& output_queue,
  WorkerContext& ctx)
{
  if (ctx.input.peek() == EOF) {
    output_queue[thread_id].close();
    return;
  }
  Record r(1, ctx.N);
  r.reference = htobe32(0);
  r.samples[0] = 0x50;
  output_queue[thread_id].push(std::move(r));
  output_queue[thread_id].close();
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
