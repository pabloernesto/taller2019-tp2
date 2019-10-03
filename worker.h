#ifndef WORKER_H_
#define WORKER_H_

#include <vector>
#include "record.h"
#include "blockingqueue.h"
#include "inputfile.h"

#include <condition_variable>

struct WorkerContext {
  InputFile& input;
  int N;
};

void do_work(
  int thread_id,
  std::vector<BlockingQueue>& output_queue,
  WorkerContext& ctx);

#endif // WORKER_H_
