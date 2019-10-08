#ifndef INPUTFILE_H_
#define INPUTFILE_H_

#include <fstream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <stdint.h>   // uint32_t

class InputFile {
  std::ifstream in;
  int T;
  std::mutex mtx;
  std::condition_variable cv;
  int turn;

  public:
  // No default constructor
  InputFile() = delete;
  InputFile(std::ifstream&& in, int T);

  // Non-copyable
  InputFile(const InputFile&) = delete;
  InputFile& operator=(const InputFile&) = delete;

  // Get (up to) n samples from the file
  // Calling thread must identify itself with its thread-id
  std::vector<uint32_t> GetSamples(int n, int thread);
};

#endif // INPUTFILE_H_
