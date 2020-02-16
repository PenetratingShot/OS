#include "kernel.h"

multiboot_memory_map_t* mmap;
uint32_t largestUseableMem = 0;

extern uint64_t __kernel_end;

void kmain(multiboot_info_t* mbd) {
  disableCursor();

  isr_install();
  irq_install();

  initMem(mbd);

  sprint("\nBitmap Addr: ");
  sprint_int((uint64_t)bitmap);
  sprint("\nMBD Total Mem: ");
  sprint_uint((uint64_t)mbd->mem_upper);
  sprint("\n");

  clear();
  drawLogo();
  wait_s(2);

  clear();

  // formatTime(); rewrite this function
  read_rtc();

  kprint("flameOS\n");

  kprint("It is ");
  // kprint(format);
  kprint_int(second);
  kprint(".");
  kprint("\nuser@flameOS # ");

  enableCursor();
}

void user_input(char* input) {
  if (strcmp(input, "halt") == 0) {
    kprint("Halting CPU");
    asm volatile("hlt");
  } else if (strcmp(input, "time") == 0) {
    formatTime();

    kprint(format);
  } else if (strcmp(input, "clear") == 0) {
    clear();

    formatTime();

    kprint("flameOS\n");

    kprint("It is ");
    kprint(format);
    kprint(".");
  } else if (strcmp(input, "testMem") == 0) {
    uint64_t* addr = (uint64_t*)malloc(35);
    sprint("Addr: ");
    sprint_uint((uint64_t)addr);
    sprint("\n");
  } else if (strcmp(input, "res") == 0) {
    getResolution();
  } else if (strcmp(input, "neofetch") == 0) {
    neofetch(largestUseableMem);
  } else if (strcmp(input, "uptime") == 0) {
    calculateUptime();
  } else if (strcmp(input, "drives") == 0) {
    drive_scan();
    if (hdd.mp == 0 || hdd.mp48 == 0) {
      kprint("Primary IDE, Master Drive (Drive 1): Online\n");
    } else {
      kprint("Primary IDE, Master Drive (Drive 1): Offline\n");
    }
    if (hdd.ms == 0 || hdd.ms48 == 0) {
      kprint("Primary IDE, Slave Drive (Drive 2): Online\n");
    } else {
      kprint("Primary IDE, Slave Drive (Drive 2): Offline\n");
    }
    if (hdd.sp == 0 || hdd.sp48 == 0) {
      kprint("Secondary IDE, Master Drive (Drive 3): Online\n");
    } else {
      kprint("Secondary IDE, Master Drive (Drive 3): Offline\n");
    }
    if (hdd.ss == 0 || hdd.ss48 == 0) {
      kprint("Secondary IDE, Slave Drive (Drive 4): Online\n");
    } else {
      kprint("Secondary IDE, Slave Drive (Drive 4): Offline\n");
    }
  } else if (strcmp(input, "read") == 0) {
    read_disk(0);
  } else if (strcmp(input, "driveTest") == 0) {
    writeIn[0] = 0x1111;
    write(0, 0);
  } else {
    if (strcmp(input, "") == 0) {

    } else {
      kprint("Unrecognized command: ");
      kprint(input);
    }
  }
  kprint("\nuser@flameOS # ");
}