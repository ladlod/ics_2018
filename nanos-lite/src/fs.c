#include "fs.h"

extern size_t serial_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_width() * screen_height() * 4;
}

int fs_open(const char *pathname, int flags, int mode){
  return -1;
}
int fs_close(int fd){
  return 0;
}
size_t fs_size(int fd){
  return file_table[fd].size;
}
size_t fs_read(int fd, void *buf, size_t len){
  return len;
}
size_t fs_write(int fd,const void *buf, size_t len){
  switch(fd){
    case FD_STDIN:
      break;
    case FD_STDOUT:
    case FD_STDERR:
      file_table[fd].write(buf, 0, len);
      break;
    default:
      break;
  }


  return len;
}
