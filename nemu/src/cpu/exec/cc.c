#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O: //0
      *dest = cpu.EFLAGS.OF;
      break;
    case CC_B: //2
      *dest = cpu.EFLAGS.CF;
      break;
    case CC_E: //4
      //printf("zf=%d\n", cpu.EFLAGS.ZF);
      *dest = cpu.EFLAGS.ZF;
      break;
    case CC_BE: //6
      *dest = cpu.EFLAGS.CF || cpu.EFLAGS.ZF;
      break;
    case CC_S: //8
      *dest = cpu.EFLAGS.SF;
      break;
    case CC_L: //c
      *dest = (cpu.EFLAGS.SF != cpu.EFLAGS.OF);
      break;
    case CC_LE: //e
      *dest = ((cpu.EFLAGS.ZF) || (cpu.EFLAGS.SF != cpu.EFLAGS.OF));
      break;
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
