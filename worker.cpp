#include "worker.h"

#include <fstream>

void do_work(
  int thread_id,
  BlockingQueue& output_queue,
  WorkerContext& ctx)
{
  if (ctx.input.peek() == EOF) return;
}

BlockingQueue::BlockingQueue(int size) : mtx(), cv(), q(), max_size(size) {}

BlockingQueue::BlockingQueue(BlockingQueue&& other)
  : mtx(other.mtx),
    cv(other.cv),
    q(std::move(other.q)),
    max_size(other.max_size)
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
  q.push(std::move(x));
}

Record&& BlockingQueue::pop() {
  Record result = std::move(q.front());
  q.pop();
  return std::move(result);
}

void BlockingQueue::close() {}

bool BlockingQueue::empty() {
  return false;
}
