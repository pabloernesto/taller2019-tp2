#include "writer.h"

#include <vector>

void do_write(
  std::ofstream& output,
  std::vector<BlockingQueue> &queues)
{
  Record r = queues[0].pop();
  output.write((char *) &r.reference, 4);
  output.write((char *) &r.sample_size_bits, 1);
  // n = 4
  const int nbits = r.sample_size_bits * 4;
  const int nbytes = nbits / 8 + (nbits % 8 ? 1 : 0);
  output.write((char *) r.samples, nbytes);
  output.close();
}
