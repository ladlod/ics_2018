#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  int64_t value;

} WP;

void init_wp_pool();
WP *new_wp(int64_t addr);
void free_wp(int n);
void print_wp();

#endif
