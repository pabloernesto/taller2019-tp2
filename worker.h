#ifndef WORKER_H_
#define WORKER_H_

#include <vector>
#include "record.h"
#include "blockingqueue.h"
#include "inputfile.h"

#include <condition_variable>

class Worker {
  int thread_id;
  std::vector<BlockingQueue>& output_queue;
  InputFile& input;
  int N;
  std::thread t;

  public:
  Worker() = delete;
  Worker(int thread_id, std::vector<BlockingQueue>& output_queue,
    InputFile& input, int N);

  // Non-copyable
  Worker(const Worker&) = delete;
  Worker& operator=(const Worker&) = delete;

  void Join();

  private:
  void do_work();
};

#endif // WORKER_H_
