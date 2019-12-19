#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  int l = 0;
  while(*s){
    l++;
    s++;
  }
  return l;
}

char *strcpy(char* dst,const char* src) {
  char *ret = dst;
  while(*src){
    *dst = *src;
    dst++;
    src++;
  }
  return ret;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  char *ret = dst;
  while(*dst){
    dst++;
  }
  while(*src){
    *dst = *src;
    dst++;
    src++;
  }

  return ret;
}

int strcmp(const char* s1, const char* s2) {
  int ret = 0;
  while(ret == 0 && *s1 && *s2){
    ret = *s1 - *s2;
    s1++;
    s2++;
  }
  return ret;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
  void *ret = v;
  while(n--){
    *(char*)v = (char)c;
    v = (char*)v + 1;
  }
  return ret;
}

void* memcpy(void* out, const void* in, size_t n) {
  void *ret = out;
  char *begin1 = (char*)out;
  const char *begin2 = (const char*)in;
  size_t i = 0;
  while(i < n){
    *(begin1 + i) = *(begin2 + i);
    i++;
  }

  return ret;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char ret = 0;
  while(ret == 0 && n--){
    ret = *(char*)s1 - *(char*)s2;
    s1 = (char*)s1 + 1;
    s2 = (char*)s2 + 1;
  }
  return ret;
}

#endif
