#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  //printf("id_src:%d id_dest:%d id_reg=%d\n",id_src->val, id_dest->val, id_dest->reg);
  //reg_l(id_dest->reg) = id_src->val; 
  //printf("id_src:%d id_dest:%d id_reg=%d\n",id_src->val, reg_l(id_dest->reg), id_dest->reg);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
  //printf("push: 0x%x, width=%d\n", id_dest->val, id_dest->width);
  if(id_dest->width == 1){
    int8_t tmp = id_dest->val;
    id_dest->val = tmp;
  }
  else if(id_dest->width == 2){
    int16_t tmp = id_dest->val;
    id_dest->val = tmp;
  }
  //printf("push 0x%x\n", id_dest->val);
  rtl_push(&id_dest->val);

  print_asm_template1(push);
}

/*make_EHelper(pushl){
  printf("pushl");
  print_asm_template1(pushl);
}*/

make_EHelper(pop) {
  //TODO();
  rtl_pop(&t0);
  operand_write(id_dest, &t0);
  //printf("pop 0x%x\n", t0);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  reg_l(4) = reg_l(5);
  rtl_pop(&t0);
  reg_w(5) = t0;

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) { 
  //printf("lea 0x%x\n", id_src->addr);
  operand_write(id_dest, &id_src->addr);
  //printf("addr:%d id_dest:%d id_reg=%d\n",id_src->addr, reg_l(id_dest->reg), id_dest->reg);
  print_asm_template2(lea);
}
