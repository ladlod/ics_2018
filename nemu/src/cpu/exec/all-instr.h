#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

//control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
//data-move.c
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);
//make_EHelper(pushl);
//arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(adc);
make_EHelper(inc);
//logic.c
make_EHelper(xor);
make_EHelper(and);
make_EHelper(or);
make_EHelper(setcc);
make_EHelper(test);
//special.c
make_EHelper(nop);