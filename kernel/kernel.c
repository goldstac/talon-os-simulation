#include "kernel.h"
#include "compat.h"
#include <stdio.h>
void kernel(){
  printf("Starting Linux linux ...\n");
  sleep_ms(2000);
  printf("starting ramdisk\n");
  sleep_ms(2000);
  printf("/dev/nvme0n1p\n");
  
}
