#include "record.h"

#include <stdint.h>   // uint32_t

Record::Record(int sample_size_bits, int n)
  : used_bits(0),
    sample_size_bits(sample_size_bits),
    reference(0),
    samples_buffer_size_bytes((int) ceil(sample_size_bits*n/8.0)),
    samples(new char[samples_buffer_size_bytes]())
{}

Record::Record(Record&& other)
  : used_bits(other.used_bits),
    sample_size_bits(other.sample_size_bits),
    reference(other.reference),
    samples_buffer_size_bytes(other.samples_buffer_size_bytes),
    samples(std::move(other.samples))
{}

Record& Record::operator=(Record&& other) {
  this->used_bits = other.used_bits;
  this->reference = other.reference;
  this->sample_size_bits = other.sample_size_bits;
  this->samples_buffer_size_bytes = other.samples_buffer_size_bytes;
  this->samples = std::move(other.samples);
  return *this;
}

void Record::push_sample(uint32_t s) {
  // find current writing position
  char *p = samples.get() + used_bits / 8;

  s <<= 8 * sizeof(s) - sample_size_bits; // significant bits at begining
  s >>= used_bits % 8;                    // align to last written bit

  while (s) {
    const unsigned char high_byte = s >> (8 * sizeof(s) - 8);
    *(p++) |= high_byte;
    s <<= 8;
  }
  used_bits += sample_size_bits;
}

void Record::write_to(std::ofstream& out) {
  out.write((char *) &reference, 4);
  out.write((char *) &sample_size_bits, 1);
  out.write((char *) samples.get(), samples_buffer_size_bytes);
}
