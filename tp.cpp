#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
  if (argc == 1) return 1;

  int input_fd = open(argv[4], 0);
  if (input_fd == -1) return 1;

  int output_fd = open(argv[5], 0);
  if (output_fd == -1) return 1;
}
