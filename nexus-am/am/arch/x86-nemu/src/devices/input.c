#include <am.h>
#include <x86.h>
#include <amdev.h>

#define I8042_PORT 0x60

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  unsigned long long key_press;
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      key_press = inl(I8042_PORT);
      //kbd->keydown = key_press;
      kbd->keycode = key_press;
      if(key_press != _KEY_NONE){
        kbd->keydown = !kbd->keydown;
      }
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
