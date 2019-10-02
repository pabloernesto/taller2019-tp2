#include "record.h"

#include <stdint.h>   // uint32_t

void Record::push_sample(uint32_t s) {
  // find current writing position
  char *p = samples + used_bits / 8;

  s <<= 8 * sizeof(s) - sample_size_bits; // significant bits at begining
  s >>= used_bits % 8;                    // align to last written bit

  while (s) {
    const unsigned char high_byte = s >> (8 * sizeof(s) - 8);
    *(p++) |= high_byte;
    s <<= 8;
  }
  used_bits += sample_size_bits;
}
