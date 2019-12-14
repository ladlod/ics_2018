#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

//control.c
make_EHelper(call);
make_EHelper(ret);
//data-move.c
make_EHelper(push);
make_EHelper(lea);
//make_EHelper(pushl);
//arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
//logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(or);
//special.c
make_EHelper(nop);