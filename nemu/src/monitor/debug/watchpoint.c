#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 64

static WP wp_pool[NR_WP]; //watchpoint池
static WP *head, *free_;

void init_wp_pool() { //初始化池
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}
/* TODO: Implement the functionality of watchpoint */

WP *new_wp(int64_t addr){
  if(!free_){
    printf("There is no place to set a watchpoint!\n");
    return NULL;
  }
  WP* res = free_;
  free_ = free_ -> next;
  res -> next = head;
  head = res;
  res->value = addr;
  return res;
}
void free_wp(int n){
  if(n < 0 || n >= NR_WP){
    printf("Index %d out of range %d!\n", n, NR_WP - 1);
    return ;
  }
  WP *wp = &wp_pool[n];

  bool success = false;
  WP *p = head;
  if(p->NO == wp->NO){
    head = head->next;
    wp->next = free_;
    free_ = wp;
    success = true;
  }
  else{
    while(p != NULL){
      if(p->next->NO == wp->NO){
        p->next = p->next->next;
        wp->next = free_;
        free_ = wp;
        success = true;
        break;
     }
    }
  }
  if(success){
    printf("Delete successful!\n");
  }
  else{
    printf("The watchpoint is not exist!\n");
  }
}
void print_wp(){
  if(!head){
    printf("There is no watchpoint!\n");
    return ;
  }
  WP *p = head;
  printf("NO\tValue\n");
  while(p){
    printf("%-d\t%-d\n", p->NO, p->value);
    p = p->next;
  }
}