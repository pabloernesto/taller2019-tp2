#ifndef WRITER_H_
#define WRITER_H_

#include "worker.h"
#include <fstream>
#include <vector>

void do_write(
  int T,
  std::ofstream& output,
  std::vector<std::unique_ptr<BlockingQueue>> &queues);

#endif // WRITER_H_
