#include "kernel.h"
#include "compat.h"
#include <stdio.h>
void driver_log(char log[50]) {}
void kernel() {
  printf("Starting Linux linux ...\n");
  sleep_ms(2000);
  driver_log("[DRIVER] UEFI DRIVER LOADING");
  driver_log("[DRIVER] HDMI DRIVER LOADING");
  driver_log("[DRIVER] SEC DRIVER LOOADING");
  driver_log("[DRIVER] PEI DRIVER LOADING");
  driver_log("[DRIVER] RAM DRIVER LOADIiNG");
  driver_log("[DRIVER] NVME DRIVER LOADING");
  printf("starting ramdisk\n");
  sleep_ms(2000);
  printf("[BOOT] /dev/nvme0n1p\n");
}
