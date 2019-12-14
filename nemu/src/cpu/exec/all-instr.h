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
//arith.c
make_EHelper(sub);
//logic.c
make_EHelper(xor);
make_EHelper(and);