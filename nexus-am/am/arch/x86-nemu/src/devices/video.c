#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

#define SCREEN_PORT 0x100

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000; //内存映射起始地址

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      uint32_t screen_read;
      screen_read = inl(SCREEN_PORT);
      //printf("screen: %d\n", screen_read);
      info->width = screen_read >> 16;
      info->height = screen_read & 0xffff; 
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
      int size = screen_height() * screen_width();
      printf("size=%d", size);
      for(int i = 0; i < size; i++){
        fb[i] = i;
      }
      /*
      for(int i = 0; i < ctl->h; i++){
        memcpy(fb + (ctl->y + i)*screen_width() + ctl->x, ctl->pixels + i*ctl->w, ctl->w*4);
      }*/
      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
