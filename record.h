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

  Record(int sample_size_bits, int n);
  ~Record() { if (samples) delete samples; }

  // Move constructor
  Record(Record&& other);

  // Move assignment
  Record& operator=(Record&& other);

  // Append a (compressed) sample to the record
  void push_sample(uint32_t s);
};

#endif // RECORD_H_
