#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
  if (argc == 1) return 1;

  std::ifstream input(argv[4], std::ios::in | std::ios::binary);
  if (!input) return 1;

  std::ofstream output(argv[5], std::ios::out | std::ios::binary);
  if (!output) return 1;

  if (input.peek() == EOF) return 0;

  const char out[] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x50 };
  output.write(out, 6);
  output.close();
}
