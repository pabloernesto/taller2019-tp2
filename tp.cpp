#include "worker.h"
#include "writer.h"

#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char **argv) {
  if (argc == 1) return 1;

  std::ifstream input(argv[4], std::ios::in | std::ios::binary);
  if (!input) return 1;

  std::ofstream output(argv[5], std::ios::out | std::ios::binary);
  if (!output) return 1;

  const int T = 1;
  const int Q = 10;

  std::condition_variable cv;
  std::mutex mtx;
  struct WorkerContext ctx = { cv, mtx, input };

  // Create worker threads and output queues
  std::vector<BlockingQueue> queues;
  std::vector<std::thread> workers;
  for (int i = 0; i < T; i++) {
    queues.emplace_back(Q);
    workers.emplace_back(do_work, i, std::ref(queues), std::ref(ctx));
  }

  // Create writer thread
  std::thread writer(do_write, std::ref(output), std::ref(queues));

  // Join all threads
  for (auto& T : workers) T.join();
  writer.join();
}
