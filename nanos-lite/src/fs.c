#include "fs.h"

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);

extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
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
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  [FD_FB] = {"/dev/fb", 0, 0, 0, NULL, fb_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_width() * screen_height() * 4;
}

int fs_open(const char *pathname, int flags, int mode){
  printf("Open the file %s\n", pathname);
  for(int i = 0; i < NR_FILES; i++){
    if(strcmp(file_table[i].name, pathname) == 0){
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("Can't find the file");
  return -1;
}
int fs_close(int fd){
  return 0;
}
size_t fs_size(int fd){
  return file_table[fd].size;
}
size_t fs_read(int fd, void *buf, size_t len){
  size_t size = fs_size(fd);
  switch(fd){
    case FD_STDIN:
    case FD_STDOUT:
    case FD_STDERR:
    case FD_FB:
      break;
    default:
      if(file_table[fd].open_offset >= size){
        return 0;
      }
      if(file_table[fd].open_offset + len > size){
        len = size - file_table[fd].open_offset;
      }
      ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
      file_table[fd].open_offset += len;
      break;
  }

  return len;
}
size_t fs_write(int fd,const void *buf, size_t len){
  //size_t size = fs_size(fd);
  switch(fd){
    case FD_STDIN:
      break;
    case FD_STDOUT:
    case FD_STDERR:
      file_table[fd].write(buf, 0, len);
      break;
    case FD_FB:
      break;
    default:
      break;
  }

  return len;
}

size_t fs_lseek(int fd, size_t offest, int whence){
  return 0;
}
