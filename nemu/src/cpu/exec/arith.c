#include "cpu/exec.h"

/*    CF; //进位标志位
      ZF; //零标志位
      SF; //符号标志位
      IF; //中断允许标志位
      OF; //溢出标志位
      */

make_EHelper(add) {
  //TODO();
  rtl_sext(&t1, &id_dest->val, id_dest->width);  //t1=dest
  rtl_sext(&t2, &id_src->val, id_src->width);  //t2=src
  rtl_add(&t0, &t1, &t2);
  //printf("0x%x + 0x%x = 0x%x\n", t1, t2, t0);

  t3 = t0 < t1 || t0 < t2; //对于无符号数，和小于加数之一则有进位
  rtl_set_CF(&t3);

  t3 = (((int32_t)t1 >= 0) && ((int32_t)t2 >= 0) && ((int32_t)t0 < 0)) || //正数加正数得负数
    (((int32_t)t1 < 0) && ((int32_t)t2 < 0) && ((int32_t)t0 >= 0));      //负数加负数得正数
  rtl_set_OF(&t3);

  rtl_update_ZFSF(&t0, 4);
  operand_write(id_dest, &t0);
  //printf("width = %d id_dest= 0x%x\n", id_dest->width, reg_l(id_dest->reg));

  print_asm_template2(add);
}

make_EHelper(sub) { //dest=dest-src, src为有符号数
  //TODO();
  rtl_sext(&t1, &id_dest->val, id_dest->width);  //t1=dest
  rtl_sext(&t2, &id_src->val, id_src->width);  //t2=src
  rtl_sub(&t0, &t1, &t2); //t0=t1-t2
  //printf("sub: %d=%d-%d\n", t0, t1, t2);
  t3 = t0 > t1;
  rtl_set_CF(&t3);  //减数大于被减数时”进位“ 
  t3 = (((int32_t)t1 <= 0) && ((int32_t) t2 > 0) && ((int32_t) t0 > 0)) || //负数减正数得正数
        (((int32_t) t1 >= 0) && ((int32_t) t2 < 0) && ((int32_t) t0 < 0)); //正数减负数得负数
  rtl_set_OF(&t3);
  rtl_update_ZFSF(&t0, 4);

  operand_write(id_dest, &t0);

  print_asm_template2(sub);
}

make_EHelper(cmp) {
  //TODO();
  //printf("ebx:0x%x eax:0x%x\n", cpu.ebx, cpu.eax);
  //printf("dest:0x%x src:0x%x\n", id_dest->val, id_src->val);
  rtl_sext(&t1, &id_dest->val, id_dest->width);
	rtl_sext(&t2, &id_src->val, id_src->width);
  rtl_sub(&t0, &t1, &t2);
  //printf("t0=0x%x t1=0x%x t2=0x%x\n",t0, t1, t2);
  //printf("SF=%d\n", cpu.EFLAGS.SF);
  t3 = t0 > t1;
  rtl_set_CF(&t3);  //无符号数差大于被减数时发生”进位“ 
  //t3 = ((((int32_t)(t1) < 0) == (((int32_t)(t2) >> 31) == 0)) && (((int32_t)(t0) < 0) != ((int32_t)(t1) < 0)));
  t3 = (((int32_t)t1 <= 0) && ((int32_t) t2 > 0) && ((int32_t) t0 > 0)) || //负数减正数得正数
        (((int32_t) t1 >= 0) && ((int32_t) t2 < 0) && ((int32_t) t0 < 0)); //正数减负数得负数
  rtl_set_OF(&t3);
  rtl_update_ZFSF(&t0, 4);
  //printf("ZF:%d\n", cpu.EFLAGS.ZF);
  //printf("dest:0x%x src:0x%x\n", id_dest->val, id_src->val);

  print_asm_template2(cmp);
}

make_EHelper(inc) {
  //TODO();
  rtl_addi(&t0, &id_dest->val, 1);
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);

  print_asm_template1(inc);
}

make_EHelper(dec) {
  rtl_subi(&t0, &id_dest->val, 1);
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);

  print_asm_template1(dec);
}

make_EHelper(neg) { //求补，取反加一
  //TODO();
  rtl_mv(&t0, &id_dest->val);
	rtl_not(&t0, &t0);
	rtl_addi(&t0, &t0, 1);
	operand_write(id_dest, &t0);

	t1 = (id_dest->val != 0);
	rtl_set_CF(&t1);

	rtl_update_ZFSF(&t0, id_dest->width);
	rtl_xor(&t1, &t0, &id_dest->val);
	rtl_not(&t1, &t1);
	rtl_msb(&t1, &t1, id_dest->width);
	rtl_set_OF(&t1);

  print_asm_template1(neg);
}

make_EHelper(adc) {
  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_setrelop(RELOP_LTU, &t3, &t2, &id_dest->val);
  rtl_get_CF(&t1);
  rtl_add(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_setrelop(RELOP_LTU, &t0, &t2, &id_dest->val);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0, &t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(adc);
}

make_EHelper(sbb) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_setrelop(RELOP_LTU, &t3, &id_dest->val, &t2);
  rtl_get_CF(&t1);
  rtl_sub(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_setrelop(RELOP_LTU, &t0, &id_dest->val, &t2);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul_lo(&t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr(R_AX, &t1, 2);
      break;
    case 2:
      rtl_sr(R_AX, &t1, 2);
      rtl_shri(&t1, &t1, 16);
      rtl_sr(R_DX, &t1, 2);
      break;
    case 4:
      rtl_mul_hi(&t2, &id_dest->val, &t0);
      rtl_sr(R_EDX, &t2, 4);
      rtl_sr(R_EAX, &t1, 4);
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul_lo(&t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr(R_AX, &t1, 2);
      break;
    case 2:
      rtl_sr(R_AX, &t1, 2);
      rtl_shri(&t1, &t1, 16);
      rtl_sr(R_DX, &t1, 2);
      break;
    case 4:
      rtl_imul_hi(&t2, &id_dest->val, &t0);
      rtl_sr(R_EDX, &t2, 4);
      rtl_sr(R_EAX, &t1, 4);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
  rtl_sext(&t0, &id_src->val, id_src->width);
  rtl_sext(&t1, &id_dest->val, id_dest->width);

  rtl_imul_lo(&t2, &t1, &t0);
  operand_write(id_dest, &t2);

  print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
  rtl_sext(&t0, &id_src->val, id_src->width);
  rtl_sext(&t1, &id_src2->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul_lo(&t2, &t1, &t0);
  operand_write(id_dest, &t2);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_lr(&t0, R_AX, 2);
      rtl_div_q(&t2, &t0, &id_dest->val);
      rtl_div_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AL, &t2, 1);
      rtl_sr(R_AH, &t3, 1);
      break;
    case 2:
      rtl_lr(&t0, R_AX, 2);
      rtl_lr(&t1, R_DX, 2);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_div_q(&t2, &t0, &id_dest->val);
      rtl_div_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AX, &t2, 2);
      rtl_sr(R_DX, &t3, 2);
      break;
    case 4:
      rtl_lr(&t0, R_EAX, 4);
      rtl_lr(&t1, R_EDX, 4);
      rtl_div64_q(&cpu.eax, &t1, &t0, &id_dest->val);
      rtl_div64_r(&cpu.edx, &t1, &t0, &id_dest->val);
      break;
    default: assert(0);
  }

  print_asm_template1(div);
}

make_EHelper(idiv) {
  switch (id_dest->width) {
    case 1:
      rtl_lr(&t0, R_AX, 2);
      rtl_idiv_q(&t2, &t0, &id_dest->val);
      rtl_idiv_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AL, &t2, 1);
      rtl_sr(R_AH, &t3, 1);
      break;
    case 2:
      rtl_lr(&t0, R_AX, 2);
      rtl_lr(&t1, R_DX, 2);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_idiv_q(&t2, &t0, &id_dest->val);
      rtl_idiv_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AX, &t2, 2);
      rtl_sr(R_DX, &t3, 2);
      break;
    case 4:
      rtl_lr(&t0, R_EAX, 4);
      rtl_lr(&t1, R_EDX, 4);
      rtl_idiv64_q(&cpu.eax, &t1, &t0, &id_dest->val);
      rtl_idiv64_r(&cpu.edx, &t1, &t0, &id_dest->val);
      break;
    default: assert(0);
  }

  print_asm_template1(idiv);
}
