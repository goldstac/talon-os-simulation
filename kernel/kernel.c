#include "kernel.h"
#include "compat.h"
#include "initramfs.h"
#include <stdio.h>
void kernel() {
  printf("[Kernel] starting inital initramfs ramdisk\n");
  create_tmpfs_initramfs();
  sleep_ms(2000);
  printf("[BOOT] /dev/nvme0n1p\n");
}
