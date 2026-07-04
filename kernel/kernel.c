#include "compat.h"
#include <stdio.h>
int main() {
  printf("Starting Linux\n");
  sleep_ms(2000);
  printf("starting ramdisk\n");
  sleep_ms(2000);
  printf("/dev/nvme0n1p\n");
  return 0;
}
