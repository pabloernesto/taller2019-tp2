#include "writer.h"

#include <vector>

void do_write(
  std::ofstream& output,
  std::vector<BlockingQueue> &queues)
{
  const char out[] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x50 };
  output.write(out, 6);
  output.close();
}
