#include "inputfile.h"

std::vector<uint32_t> InputFile::GetSamples(int n, int turn) {
  std::vector<uint32_t> result(n);
  auto data = result.data();

  in.read((char*) data, 4*n);

  result.resize(in.gcount() / 4);
  return result;
}
