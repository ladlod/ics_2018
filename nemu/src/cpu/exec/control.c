#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = decoding.opcode & 0xf;
  printf("cc:0x%x", cc);
  rtl_setcc(&t0, cc);
  //printf("SF:%d\n", cpu.EFLAGS.SF);
  rtl_li(&t1, 0);
  printf("t0:%d t1:%d\n", t0, t1);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);
  //rtl_j(decoding.jmp_eip);
  printf("is_jmp:%d eip:0x%x\n",decoding.is_jmp, cpu.eip);

  printf("j%s %x\n", get_cc_name(cc), decoding.jmp_eip);
  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  //TODO();

  rtl_push(&(decoding.seq_eip));
  rtl_j(decoding.jmp_eip);

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  rtl_pop(&decoding.jmp_eip);
  //printf("jmp to 0x%x\n", decoding.jmp_eip);
  rtl_j(decoding.jmp_eip);

  print_asm("ret");
}

make_EHelper(call_rm) {
  //TODO();
  rtl_push(&(decoding.seq_eip));
  rtl_jr(&id_dest->val);

  print_asm("call *%s", id_dest->str);
}
