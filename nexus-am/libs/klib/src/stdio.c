#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char out[256];
  sprintf(out, fmt);
  int len = strlen(out);
  for(int i = 0; i < len; i++){
    _putc(out[i]);
  }
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *str = out;
  char *s;
  int num;
  char num_s[1024];
  while(*fmt){
    if(*fmt == '%'){ 
      fmt++;
      switch(*fmt){
        case 's':
          s = va_arg(ap, char*);
          for(int i = 0; i < strlen(s); i++){
            *str = *s;
            str++;
            s++;
          }
          break;
        case 'd':
            num = va_arg(ap, int);
            int j = 0;
            if(num == 0){
              num_s[j] = 0;
            }
            else{
              if(num < 0){
                *str = '-';
                num = -num;
                str++;
              }
              while(num){
                num_s[j] = num%10 + '0';
                j++;
              }
            }
            while(j--){
              *str = num_s[j];
              str++;
            }
            break;
      }      
    }
    else{
      *str = *fmt;
      str++;
    }
    fmt++;
  };

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
