#include "common.h"
#include "syscall.h"
#include "fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
  //printf("a0=%d\n", a[0]);

  switch (a[0]) {
    case SYS_exit: 
      _halt(0); //code=？
      c->GPRx = 1;
      break;
    case SYS_yield: 
      _yield();
      c->GPRx = 1;
      break;
    case SYS_open:
      break;
    case SYS_write:
      //c->GPRx = fs_write(a[1], (void*)a[2], a[3]);
      for(int i = 0; i < a[3]; i++){
        _putc(a[2] + i);
      }
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
