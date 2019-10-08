#include "writer.h"

#include <vector>

void do_write(
  int T,
  std::ofstream& output,
  std::vector<BlockingQueue> &queues)
{
  for (int worker = 0; !queues[worker].isClosed(); worker = (worker + 1) % T) {
    Record&& r = queues[worker].pop();
    r.write_to(output);
  }
  output.close();
}
