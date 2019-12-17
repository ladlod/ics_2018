#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

//control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(call_rm);
make_EHelper(jmp_rm);
//data-move.c
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);
make_EHelper(pop);
make_EHelper(cltd);
make_EHelper(movsx);
//make_EHelper(pushl);
//arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
make_EHelper(inc);
make_EHelper(cmp);
make_EHelper(dec);
make_EHelper(imul2);
make_EHelper(idiv);
make_EHelper(sbb);
make_EHelper(mul);
make_EHelper(imul1);
make_EHelper(div);
make_EHelper(neg);
//logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(or);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(sar);
make_EHelper(not);
make_EHelper(shl);
make_EHelper(shr);
//special.c
make_EHelper(nop);
//system.c
make_EHelper(in);
make_EHelper(out);
