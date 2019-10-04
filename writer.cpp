#include "writer.h"

#include <vector>

void do_write(
  std::ofstream& output,
  std::vector<BlockingQueue> &queues)
{
  while (!queues[0].isClosed()) {
    Record&& r = queues[0].pop();
    r.write_to(output);
  }
  output.close();
}
