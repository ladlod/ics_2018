#include "proc.h"
#include "fs.h"

#define DEFAULT_ENTRY 0x4000000

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  ramdisk_read((void *)DEFAULT_ENTRY, 0, get_ramdisk_size());
  //Log("done ramdisk read");
  //TODO();
  //int fd = fs_open(filename, 0, 0);
  //printf("fd=%d\n", fd);
  //fs_read(fd, (void*)DEFAULT_ENTRY, fs_size(fd));
  //fs_close(fd);

  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
