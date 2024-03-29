#include "writer.h"

#include <vector>

void do_write(
  int T,
  std::ofstream& output,
  std::vector<std::unique_ptr<BlockingQueue>> &queues)
{
  for (int worker = 0; queues[worker]->isPopable(); worker = (worker + 1) % T) {
    Record&& r = queues[worker]->pop();
    r.write_to(output);
  }
  output.close();
}
