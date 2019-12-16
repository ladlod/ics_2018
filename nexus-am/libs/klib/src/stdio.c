#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char out[256];
  sprintf(out, fmt);
  int len = strlen(out);
  for(int i = 0; i < out; i++){
    _putc(out[i]);
  }
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  while(*fmt){
    fmt++;
  }

  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  int  val;
  va_start(args,fmt);
  val = vsprintf(out,fmt,args);
  va_end(args);
  return val;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
