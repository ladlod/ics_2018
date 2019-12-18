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
make_EHelper(cwtl);
make_EHelper(pusha);
make_EHelper(popa);
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
make_EHelper(rol);
//special.c
make_EHelper(nop);
//system.c
make_EHelper(in);
make_EHelper(out);
make_EHelper(lidt);
make_EHelper(int);


/*make_EHelper(notrack){
    vaddr_t addr;
    vaddr_read(addr, );
    printf("notrack jmp to 0x%x\n", vaddr_read());
    rtl_j(cpu.eip + 8);
}*/

make_EHelper(endbr32){
    rtl_j(cpu.eip + 4);
    //print_asm(endbr32);
}

/*make_EHelper(stos){
  if(id_dest->width == 4){
    t0 = vaddr_read(cpu.cs+cpu.edi, 4);
    rtl_lr(&cpu.eax, t0, 4);
    if(cpu.EFLAGS.DF == 0){
      rtl_addi(&cpu.edi, &cpu.edi, 4);
    }
    else{
      rtl_subi(&cpu.edi, &cpu.edi, 4);
    }
  }
	else if (id_dest->width == 2)
{
	t0 = vaddr_read(cpu.es+reg_w(R_DI), 2);
	rtl_lr(&cpu.eax, t0, 2);
	if(cpu.eflags.DF == 0){
		rtl_addi(&cpu.edi, &cpu.edi, 2);
	}
	else{
		rtl_subi(&cpu.edi, &cpu.edi, 2);
	}
}
else{
	t0 = vaddr_read(cpu.es+reg_w(R_DI), 1);
	rtl_lr(&cpu.eax, t0, 1);
	if(cpu.eflags.DF == 0){
		rtl_addi(&cpu.edi, &cpu.edi, 1);
	}
	else{
		rtl_subi(&cpu.edi, &cpu.edi, 1);
	}
    }
    print_asm("stos");
}*/