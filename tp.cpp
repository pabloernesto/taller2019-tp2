#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "worker.h"
#include "writer.h"
#include "inputfile.h"

int main(int argc, char **argv) {
  if (argc == 1) return 1;

  std::ifstream input_file(argv[4], std::ios::in | std::ios::binary);
  if (!input_file) return 1;

  std::ofstream output(argv[5], std::ios::out | std::ios::binary);
  if (!output) return 1;

  const int N = std::stol(argv[1]);
  const int T = std::stol(argv[2]);
  const int Q = std::stol(argv[3]);

  InputFile input(std::move(input_file), T);

  // Create worker threads and output queues
  std::vector<std::unique_ptr<BlockingQueue>> queues;
  std::vector<std::unique_ptr<Worker>> workers;
  for (int i = 0; i < T; i++) {
    queues.emplace_back(new BlockingQueue(Q));
    workers.emplace_back(new Worker(i, queues, input, N));
  }

  // Create writer thread
  std::thread writer(do_write, T, std::ref(output), std::ref(queues));

  // Join all threads
  for (auto& w : workers) w->Join();
  writer.join();
}
