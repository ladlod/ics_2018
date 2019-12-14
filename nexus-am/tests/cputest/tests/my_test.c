#include "trap.h"

int test(int i){
    return i + 1;
}

int main(){
    int i = 1;
    int j = test(i);
    nemu_assert(j == 2);
    return 0;
}