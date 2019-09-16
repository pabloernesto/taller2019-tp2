#ifndef WRITER_H_
#define WRITER_H_

#include "worker.h"
#include <fstream>
#include <vector>

void do_write(
  std::ofstream& output,
  std::vector<BlockingQueue> &queues);

#endif // WRITER_H_
