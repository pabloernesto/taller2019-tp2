#ifndef BLOCKINGQUEUE_H_
#define BLOCKINGQUEUE_H_

#include "record.h"
#include <stdint.h>   // uint32_t
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class BlockingQueue {
  std::mutex mtx;
  std::condition_variable empty_cv, full_cv;
  std::queue<Record> q;
  size_t max_size;
  bool closed;

  public:
  explicit BlockingQueue(int size);

  // Non-copyable
  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

  void push(Record&& x);
  Record pop();
  void close();
  bool isPopable();
};

#endif // BLOCKINGQUEUE_H_
