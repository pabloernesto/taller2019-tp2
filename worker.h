#ifndef WORKER_H_
#define WORKER_H_

#include <vector>
#include "record.h"
#include "blockingqueue.h"

#include <condition_variable>

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
