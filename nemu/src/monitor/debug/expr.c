#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ, TK_NEQ, TK_AND, TK_OR, TK_SUB, TK_PLUS, TK_MINUS, TK_G, TK_L,  TK_GE, TK_LE ,
//等于 257,不等于 258,与 259,或 260,减 261,加 262,负数 263,大于 264,小于 265,大于等于 266,小于等于 267
  TK_MUL, TK_DIV, TK_NOT, TK_LB, TK_RB,
//乘 268,除 269,取反 270,左括号 271,右括号 272
  TK_POINT, TK_NUMBER, TK_HEX
//指针 273,数字 274,十六进制 275
/* TODO: Add more token types */
};

static struct rule {
  char *regex;
  int token_type;
  int level;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE, 0},    //空格
  {"\\b[0-9]+\\b", TK_NUMBER, 0}, //数字
  {"\\b0[xX][0-9a-fA-F]+\\b", TK_HEX, 0}, //十六进制
  //{"\\$(eax|EAX|ebx|EBX|ecx|ECX|edx|EDX|ebp|EBP|esp|ESP|esi|ESI|edi|EDI|eip|EIP|([ABCD][HLX])|([abcd][hlx]))", TK_REGISTER, 0}, //寄存器
  {"\\t+", TK_NOTYPE, 0},  //tab
  {"\\|\\|", TK_OR, 1}, //或
  {"&&", TK_AND, 2}, //与
  {"==", TK_EQ, 3},         //等于
  {"!=", TK_NEQ, 3},        //不等于
  {"<=", TK_LE, 3}, //小于等于
  {">=", TK_GE, 3}, //大于等于
  {"<", TK_L, 3}, //小于
  {">", TK_G, 3}, //大于
  {"\\+", TK_PLUS, 4},  // 加号
  {"-", TK_SUB, 4}, //减号
  {"\\*", TK_MUL, 5}, //乘号
  {"/", TK_DIV, 5}, //除号
  {"!", TK_NOT, 6}, //取反
  {"\\(", TK_LB, 7}, //左括号
  {"\\)", TK_RB, 7}  //右括号
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) ) //rules的个数

static regex_t re[NR_REGEX]; 

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
  int level;
} Token;

Token tokens[1024];
int nr_token; //token的个数

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE: break;
          /*case TK_REGISTER: 
            tokens[nr_token].type = rules[i].token_type;
            tokens[nr_token].level = rules[i].level;
            strncpy (tokens[nr_token].str, substr_start + 1, substr_len-1);
						tokens[nr_token].str[substr_len-1]='\0';
						nr_token++;
            break;*/
          default: 
            tokens[nr_token].type = rules[i].token_type;
            tokens[nr_token].level = rules[i].level;
            strncpy (tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len + 1]='\0';
						nr_token++;
            break;
        }
        //position += substr_len;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool checkParentheses(int l, int r){
  if(tokens[l].type == TK_LB && tokens[r].type == TK_RB){
    int n = 0; //记录除初始括号外单独的左括号数目，每次配对减一，小于0的时候返回false
    for(int i = l + 1; i < r; i++){
      if(tokens[i].type == TK_LB) n++;
      else if(tokens[i].type == TK_RB) n--;
      if(n < 0){
        return false;
      }
    }
    if(n == 0)
      return true;
  }
  return false;
}

int findOperator(int left, int right){
  int min_level = 8;
  int op = left;
  int n = 0;
  for(int i = left; i <= right; i++){
    if(tokens[i].type == TK_NUMBER || tokens[i].type == TK_HEX)
      continue;
    else if(tokens[i].type == TK_LB)
      n++;
    else if(tokens[i].type == TK_RB)
      n--;
    if(n != 0)
      continue;
    if(tokens[i].level < min_level){
      min_level = tokens[i].level;
      op = i;
    }
  }
  return op;
}

uint32_t caculation(int left, int right, bool *success){
  if(!(*success))
    return -1;
  if(left > right){
    printf("Error left > right \n");
    return -1;
  }
  else if(left == right){
    int num;
    switch (tokens[left].type)
    {
    case TK_NUMBER:
      num = atoi(tokens[left].str);
      //printf("%s %d\n", tokens[left].str, num);
      return num;
    case TK_HEX:
      sscanf(tokens[left].str, "%x", &num);
      //printf("%s %d\n", tokens[left].str, num);
      return num;
    default:
      success = false;
      printf("Unkown type!\n");
      return -1;
    }
  }
  else if(checkParentheses(left, right)){
    return caculation(left + 1, right - 1, success);
  }
  else{
    int op = findOperator(left, right);
    //printf("op=%d\n", op);
    if(op == left || tokens[op].type == TK_NOT || tokens[op].type == TK_POINT || tokens[op].type == TK_MINUS){
      uint32_t num = caculation(left + 1, right, success);
      switch (tokens[op].type)
      {
      case TK_NOT: //取非
        return !num;
      case TK_POINT: //指针
        return vaddr_read(num, 4);
      case TK_MINUS: //取负
        return -num;
      default:
        printf("Illegal opreator at the front the expression!\n");
        *success = false;
        return -1;
      }
    }
    uint32_t num1 = caculation(left, op - 1, success);
    uint32_t num2 = caculation(op + 1, right, success);
    //printf("num1=%d num2=%d\n", num1, num2);
    switch (tokens[op].type)
    {
    case TK_EQ: //相等
      return num1 == num2;
    case TK_NEQ: //不等
      return num1 != num2;
    case TK_AND: //与
      return num1 && num2;
    case TK_OR: //或
      return num1 || num2;
    case TK_SUB: //减
      return num1 - num2;
    case TK_PLUS: //加
      return num1 + num2;
    case TK_G: //大于
      return num1 > num2;
    case TK_L: //小于
      return num1 < num2;
    case TK_GE: //大于等于
      return num1 >= num2;
    case TK_LE: //小于等于
      return num1 <= num2;
    case TK_MUL: //乘
      return num1 * num2;
    case TK_DIV: //除
      if(num2 == 0){
        printf("Illegal divisor shouldn't be 0!\n");
        *success = false;
        return -1;
      }
      return num1 / num2;    
    default:
      printf("Illegal opreator unkonwn!\n");
      *success = false;
      return -1;
    }
  }
  return -1;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }


  *success = true;
  /* TODO: Insert codes to evaluate the expression. */
  //处理指针和负号
  for(int i = 0; i < nr_token; i++){
    if(tokens[i].type == TK_MUL && ((i == 0) || (tokens[i - 1].type != TK_NUMBER && tokens[i - 1].type != TK_HEX
      && tokens[i - 1].type != TK_RB))){ //指针
      tokens[i].type = TK_POINT;
      tokens[i].level = 6;
    }
    if(tokens[i].type == TK_DIV && ((i == 0) || (tokens[i - 1].type != TK_NUMBER && tokens[i - 1].type != TK_HEX
      && tokens[i - 1].type != TK_RB))){ //负号
      tokens[i].type = TK_MINUS;
      tokens[i].level = 6;
    }
  }
  /*for(int i = 0; i < nr_token; i++){
    printf("%d\n", tokens[i].type);
  }*/

  return caculation(0, nr_token - 1, success);
}
