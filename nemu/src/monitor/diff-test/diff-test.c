#include <dlfcn.h>

#include "nemu.h"
#include "monitor/monitor.h"
#include "diff-test.h"

static void (*ref_difftest_memcpy_from_dut)(paddr_t dest, void *src, size_t n);
static void (*ref_difftest_getregs)(void *c);
static void (*ref_difftest_setregs)(const void *c);
static void (*ref_difftest_exec)(uint64_t n);

static bool is_skip_ref;
static bool is_skip_dut;

void difftest_skip_ref() { is_skip_ref = true; }
void difftest_skip_dut() { is_skip_dut = true; }

void init_difftest(char *ref_so_file, long img_size) {
#ifndef DIFF_TEST
  return;
#endif

  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY | RTLD_DEEPBIND);
  assert(handle);

  ref_difftest_memcpy_from_dut = dlsym(handle, "difftest_memcpy_from_dut");
  assert(ref_difftest_memcpy_from_dut);

  ref_difftest_getregs = dlsym(handle, "difftest_getregs");
  assert(ref_difftest_getregs);

  ref_difftest_setregs = dlsym(handle, "difftest_setregs");
  assert(ref_difftest_setregs);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  void (*ref_difftest_init)(void) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: \33[1;32m%s\33[0m", "ON");
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in include/common.h.", ref_so_file);

  ref_difftest_init();
  ref_difftest_memcpy_from_dut(ENTRY_START, guest_to_host(ENTRY_START), img_size);
  ref_difftest_setregs(&cpu);
}

void difftest_step(uint32_t eip) {
  CPU_state ref_r;

  if (is_skip_dut) {
    is_skip_dut = false;
    return;
  }

  if (is_skip_ref) {
    // to skip the checking of an instruction, just copy the reg state to reference design
    ref_difftest_setregs(&cpu);
    is_skip_ref = false;
    return;
  }

  ref_difftest_exec(1);
  ref_difftest_getregs(&ref_r);

  // TODO: Check the registers state with the reference design.
  // Set `nemu_state` to `NEMU_ABORT` if they are not the same.
  //printf("test");
  Assert(ref_r.eax == cpu.eax, "eax should be 0x%x, not 0x%x", ref_r.eax, cpu.eax);
  Assert(ref_r.ebp == cpu.ebp, "ebp should be 0x%x, not 0x%x", ref_r.ebp, cpu.ebp);
  Assert(ref_r.ebx == cpu.ebx, "ebx should be 0x%x, not 0x%x", ref_r.ebx, cpu.ebx);
  Assert(ref_r.ecx == cpu.ecx, "ecx should be 0x%x, not 0x%x", ref_r.ecx, cpu.ecx);
  Assert(ref_r.edi == cpu.edi, "edi should be 0x%x, not 0x%x", ref_r.edi, cpu.edi);
  Assert(ref_r.edx == cpu.edx, "edx should be 0x%x, not 0x%x", ref_r.edx, cpu.edx);
  Assert(ref_r.eip == cpu.eip, "eip should be 0x%x, not 0x%x", ref_r.eip, cpu.eip);
  Assert(ref_r.esi == cpu.esi, "esi should be 0x%x, not 0x%x", ref_r.esi, cpu.esi);
  Assert(ref_r.esp == cpu.esp, "esp should be 0x%x, not 0x%x", ref_r.esp, cpu.esp);
  Assert(ref_r.EFLAGS.CF == cpu.EFLAGS.CF, "CF should be 0x%x, not 0x%x", ref_r.EFLAGS.CF, cpu.EFLAGS.CF);
  Assert(ref_r.EFLAGS.OF == cpu.EFLAGS.OF, "OF should be 0x%x, not 0x%x", ref_r.EFLAGS.OF , cpu.EFLAGS.OF);
  Assert(ref_r.EFLAGS.SF == cpu.EFLAGS.SF, "SF should be 0x%x, not 0x%x", ref_r.EFLAGS.SF , cpu.EFLAGS.SF);
  Assert(ref_r.EFLAGS.ZF == cpu.EFLAGS.ZF, "ZF should be 0x%x, not 0x%x\neax=0x%x ebx=0x%x", ref_r.EFLAGS.ZF , cpu.EFLAGS.ZF, ref_r.eax, ref_r.ebx);
  
}
