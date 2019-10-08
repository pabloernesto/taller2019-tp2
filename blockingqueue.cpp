#include "blockingqueue.h"

BlockingQueue::BlockingQueue(int size)
  : mtx(new std::mutex),
  empty_cv(new std::condition_variable),
  full_cv(new std::condition_variable),
  q(),
  max_size(size),
  closed(false)
{}

BlockingQueue::BlockingQueue(BlockingQueue&& other)
  : mtx(other.mtx),
    empty_cv(other.empty_cv),
    full_cv(other.full_cv),
    q(std::move(other.q)),
    max_size(other.max_size),
    closed(other.closed)
{
  other.mtx = nullptr;
  other.empty_cv = nullptr;
  other.full_cv = nullptr;
}

BlockingQueue& BlockingQueue::operator=(BlockingQueue&& other) {
  this->mtx = other.mtx;
  other.mtx = nullptr;
  this->empty_cv = other.empty_cv;
  other.empty_cv = nullptr;
  this->full_cv = other.full_cv;
  other.full_cv = nullptr;
  this->q = std::move(other.q);
  this->max_size = other.max_size;
  return *this;
}

BlockingQueue::~BlockingQueue() {
  if (this->mtx) delete(this->mtx);
  if (this->empty_cv) delete(this->empty_cv);
  if (this->full_cv) delete(this->full_cv);
}

void BlockingQueue::push(Record&& x) {
  std::unique_lock<std::mutex> lock(*mtx);
  while (q.size() == max_size)
    full_cv->wait(lock);
  q.push(std::move(x));
  empty_cv->notify_one();
}

Record BlockingQueue::pop() {
  std::unique_lock<std::mutex> lock(*mtx);
  while ((q.size() == 0) && !closed)
    empty_cv->wait(lock);

  if (q.size() == 0) throw std::runtime_error("trying to pop an empty queue");

  Record result = std::move(q.front());
  q.pop();
  full_cv->notify_one();
  return result;
}

void BlockingQueue::close() {
  std::unique_lock<std::mutex> lock(*mtx);
  closed = true;
  empty_cv->notify_one();
}

bool BlockingQueue::isPopable() {
  std::unique_lock<std::mutex> lock(*mtx);
  while ((q.size() == 0) && !closed)
    empty_cv->wait(lock);

  return q.size() != 0;
}
