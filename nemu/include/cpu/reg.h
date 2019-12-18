#ifndef __REG_H__
#define __REG_H__

#include "common.h"

//寄存器
enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };  //32bits
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };  //16bits
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };  //8bits

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {

  /*struct {
    uint32_t _32;
    uint16_t _16;
    uint8_t _8[2];
  } gpr[8];*/

  /* Do NOT change the order of the GPRs' definitions. */
union{
  struct {
    rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
  };
  union{
    paddr_t _32;
    ioaddr_t _16;
    bool _8[2];
  } gpr[8];
};

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
  //rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

  vaddr_t eip;

  union{
    struct {
      uint8_t CF : 1; //进位标志位
      uint8_t ZF : 1; //零标志位
      uint8_t SF : 1; //符号标志位
      uint8_t IF : 1; //中断允许标志位
      uint8_t OF : 1; //溢出标志位
    } EFLAGS;
    uint32_t falgs;
  };

  struct {
    uint16_t len; //长度
    uint32_t base; //首地址
  } IDTR;

  uint16_t cs; //代码段寄存器

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 8);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

static inline const char* reg_name(int index, int width) {
  assert(index >= 0 && index < 8);
  switch (width) {
    case 4: return regsl[index];
    case 1: return regsb[index];
    case 2: return regsw[index];
    default: assert(0);
  }
}

#endif
