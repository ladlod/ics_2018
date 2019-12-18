#include "cpu/exec.h"
#include "memory/mmu.h"


void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
  union{
    GateDesc g;
    struct{
      uint32_t lo, hi;
    };
  } item;
  vaddr_t addr;
  addr = 8 * NO + cpu.IDTR.base;
  item.lo = vaddr_read(addr, 4);
  item.hi = vaddr_read(addr + 4, 4);

  t1 = (item.g.offset_15_0 & 0xffff) | ((item.g.offset_31_16 & 0xffff) << 16);
  printf("t1: 0x%x\n", t1);
  rtl_j(t1);

  rtl_push(&cpu.falgs); //标志位寄存器入栈
  t0 = cpu.cs;
  rtl_push(&t0); //代码段寄存器入栈
  rtl_push(&ret_addr); //返回地址入栈

  //cpu.EFLAGS.IF = 0; //不允许中断
}

void dev_raise_intr() {
}
