#ifndef RECORD_H_
#define RECORD_H_

#include <stdint.h>   // uint32_t
#include <cmath>
#include <fstream>    // ofstream
#include <memory>     // unique_ptr

class Record {
  int used_bits;
  unsigned char sample_size_bits;

  public:
    unsigned int reference;

  private:
  int samples_buffer_size_bytes;
  std::unique_ptr<char> samples;

  public:
  // Non-copyable
  Record(const Record&) = delete;
  Record& operator=(const Record&) = delete;

  // No default constructor
  Record() = delete;

  Record(int sample_size_bits, int n);

  // Move constructor
  Record(Record&& other);

  // Move assignment
  Record& operator=(Record&& other);

  // Append a (compressed) sample to the record
  void push_sample(uint32_t s);

  void write_to(std::ofstream& out);
};

#endif // RECORD_H_
