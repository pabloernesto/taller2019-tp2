#ifndef WORKER_H_
#define WORKER_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include "record.h"

class BlockingQueue {
  std::mutex *mtx;
  std::condition_variable *cv;
  std::queue<Record> q;
  int max_size;
  bool closed;

  public:
  explicit BlockingQueue(int size);
  ~BlockingQueue();

  // Non-copyable
  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

  // Move-constructable
  BlockingQueue(BlockingQueue&& other);
  BlockingQueue& operator=(BlockingQueue&& other);

  void push(Record&& x);
  Record pop();
  void close();
  bool isClosed();
};

struct WorkerContext {
  std::condition_variable& cv;
  std::mutex& lock;
  std::ifstream& input;
  int N;
};

void do_work(
  int thread_id,
  std::vector<BlockingQueue>& output_queue,
  WorkerContext& ctx);

#endif // WORKER_H_
