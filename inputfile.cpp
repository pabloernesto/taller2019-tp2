#include "inputfile.h"

#include <vector>
#include <mutex>    // mutex, unique_lock

InputFile::InputFile(std::ifstream&& in, int T)
  : in(std::move(in)), T(T), mtx(), cv(), turn(0)
{}

std::vector<uint32_t> InputFile::GetSamples(int n, int thread) {
  std::unique_lock<std::mutex> lock(mtx);
  while (thread != turn)
    cv.wait(lock);

  std::vector<uint32_t> result(n);
  auto data = result.data();

  in.read((char*) data, 4*n);
  result.resize(in.gcount() / 4);

  // update turn counter and notify
  turn = (turn + 1) % T;
  cv.notify_all();

  return result;
}
