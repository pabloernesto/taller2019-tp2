#ifndef RECORD_H_
#define RECORD_H_

#include <stdint.h>   // uint32_t
#include <cmath>

class Record {
  int used_bits;

  public:
  unsigned char sample_size_bits;
  unsigned int reference;
  char *samples;

  // Non-copyable
  Record(const Record&) = delete;
  Record& operator=(const Record&) = delete;

  // No default constructor
  Record() = delete;

  Record(int sample_size_bits, int n)
    : used_bits(0),
      sample_size_bits(sample_size_bits),
      reference(0),
      samples(new char[(int) ceil(sample_size_bits*n/8.0)]) {}
  ~Record() { if (samples) delete samples; }

  // Move constructor
  Record(Record&& other)
    : used_bits(other.used_bits),
      sample_size_bits(other.sample_size_bits),
      reference(other.reference),
      samples(other.samples) { other.samples = nullptr; }

  // Move assignment
  Record& operator=(Record&& other) {
    this->used_bits = other.used_bits;
    this->reference = other.reference;
    this->sample_size_bits = other.sample_size_bits;
    this->samples = other.samples;
    other.samples = nullptr;
    return *this;
  }

  // Append a (compressed) sample to the record
  void push_sample(uint32_t s);
};

#endif // RECORD_H_
