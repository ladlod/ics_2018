#include "common.h"
#include "syscall.h"
#include "fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
  printf(" a0=%d a1=%d a2=%d a3=%d ", a[0], a[1], a[2], a[3]);

  switch (a[0]) {
    case SYS_exit: 
      _halt(a[2]); //code=？
      c->GPRx = 1;
      break;
    case SYS_yield: 
      _yield();
      c->GPRx = 1;
      break;
    /*case SYS_open:
      break;
    case SYS_read:
      break;*/
    case SYS_write:
      c->GPRx = fs_write(a[1], (void*)a[2], a[3]);
      break;
    case SYS_brk:
      c->GPRx = 0;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
