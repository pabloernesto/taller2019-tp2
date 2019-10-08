#include "blockingqueue.h"

BlockingQueue::BlockingQueue(int size)
  : mtx(), empty_cv(), full_cv(), q(), max_size(size), closed(false) {}

void BlockingQueue::push(Record&& x) {
  std::unique_lock<std::mutex> lock(mtx);
  while (q.size() == max_size)
    full_cv.wait(lock);

  if (closed) throw std::runtime_error("trying to push to a closed queue");

  q.push(std::move(x));
  empty_cv.notify_all();
}

Record BlockingQueue::pop() {
  std::unique_lock<std::mutex> lock(mtx);
  while ((q.size() == 0) && !closed)
    empty_cv.wait(lock);

  if (q.size() == 0) throw std::runtime_error("trying to pop an empty queue");

  Record result = std::move(q.front());
  q.pop();
  full_cv.notify_all();
  return result;
}

void BlockingQueue::close() {
  std::unique_lock<std::mutex> lock(mtx);
  closed = true;
  empty_cv.notify_all();
}

bool BlockingQueue::isPopable() {
  std::unique_lock<std::mutex> lock(mtx);
  while ((q.size() == 0) && !closed)
    empty_cv.wait(lock);

  return q.size() != 0;
}
