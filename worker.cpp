#include "worker.h"

#include <fstream>
#include <endian.h>

void do_work(
  int thread_id,
  BlockingQueue& output_queue,
  WorkerContext& ctx)
{
  if (ctx.input.peek() == EOF) return;
  Record r(1, 4);
  r.reference = htobe32(0);
  r.samples[0] = 0x50;
  output_queue.push(std::move(r));
}

BlockingQueue::BlockingQueue(int size) : mtx(), cv(), q(), max_size(size), closed(false) {}

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
  this->mtx = std::move(other.mtx);
  this->cv = std::move(other.cv);
  this->q = std::move(other.q);
  this->max_size = other.max_size;
  return *this;
}

void BlockingQueue::push(Record&& x) {
  std::unique_lock<std::mutex> lock(*mtx);
  q.push(std::move(x));
  cv->notify_one();
}

Record&& BlockingQueue::pop() {
  std::unique_lock<std::mutex> lock(*mtx);
  while (q.size() == 0 && !closed)
    cv->wait(lock);

  Record result = std::move(q.front());
  q.pop();
  cv->notify_one();
  return std::move(result);
}

void BlockingQueue::close() {
  std::unique_lock<std::mutex> lock(*mtx);
  closed = true;
  cv->notify_one();
}
