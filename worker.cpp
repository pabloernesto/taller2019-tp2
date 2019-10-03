#include "worker.h"

#include <fstream>
#include <endian.h>
#include <vector>
#include <iterator>
#include <algorithm>  // std::transform
#include <numeric>    // std::accumulate

// Find last set bit
// Returns the position of the last (most significant) bit set in i. The least
//   significant bit is position 1. This function returns 0 if no bits are set
//   in i.
// Copied (with minor stilistic adaptations) from the freeBSD
//   implementation at:
//   https://github.com/freebsd/freebsd/blob/master/sys/libkern/fls.c
static int fls(uint32_t i);

void do_work(
  int thread_id,
  std::vector<BlockingQueue>& output_queue,
  WorkerContext& ctx)
{
  while (1) {
    // collect samples
    auto samples = ctx.input.GetSamples(ctx.N, 0);
    if (samples.size() == 0) break;

    // change to little endian
    std::transform(samples.begin(), samples.end(), samples.begin(),
      [](uint32_t num){ return be32toh(num); });

    // find reference point (minimum of all samples)
    const uint32_t reference = std::accumulate(samples.begin(), samples.end(),
      samples[0], [](uint32_t a, uint32_t b){ return std::min(a, b); });

    // normalize
    std::transform(samples.begin(), samples.end(), samples.begin(),
      [reference](uint32_t num){ return num - reference; });

    // find length in bits
    std::vector<int> bit_lengths(samples.size());
    std::transform(samples.begin(), samples.end(), bit_lengths.begin(),
      [](uint32_t num){ return fls(num); });

    // find max length
    const uint32_t max_bits = std::accumulate(bit_lengths.begin(),
      bit_lengths.end(), 0, [](int a, int b){ return std::max(a, b); });

    // create record
    Record r(max_bits, ctx.N);
    r.reference = htobe32(reference);

    // load samples
    for (auto s : samples) r.push_sample(s);

    // push record into queue
    output_queue[thread_id].push(std::move(r));
  }
  output_queue[thread_id].close();
}

static int fls(uint32_t i) {
  if (!i) return 0;

  int bit;
  for (bit = 1; i != 1; bit++) i >>= 1;
  return bit;
}
