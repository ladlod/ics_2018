#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) { //continue
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) { //quit
  return -1;
}

static int cmd_help(char *args); //help completed
static int cmd_si(char *args); //single completed
static int cmd_info(char *args); //info 
static int cmd_p(char *args); //caculation completed
static int cmd_x(char *args); //scan memory completed
static int cmd_w(char *args); //set watchpoint
static int cmd_d(char *args); //delete watchpoiot

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  {"si", "Single step for n", cmd_si},
  {"info", "Print the regsters or watchpoint", cmd_info},
  {"p", "Caculate the expression", cmd_p},
  {"x", "Scan the memory", cmd_x},
  {"w", "Set a watchpoint", cmd_w},
  {"d", "Delete a watchpoint", cmd_d}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) { //help
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){ //single step
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
    cpu_exec(1);
    return 0;
  }
  char *leftover;
  unsigned long n = strtoul(arg, &leftover, 10);
  if(*leftover != '\0'){
    printf("Please input the right arguement!\n");
  }else{
    cpu_exec(n);
  }

  return 0;
}

static void printRegster(int size, int index){
  switch(size) {
		case 1: printf("%s\t%#04X\n", reg_name(index, 1), reg_b(index)); break;
		case 2: printf("%s\t%#06X\n", reg_name(index, 2), reg_w(index)); break;
		case 4: printf("%s\t%#010X\n", reg_name(index, 4), reg_l(index)); break;
		case -1: printf("eip\t%#010X\n", cpu.eip); break;
    default: break;
	}
}

static int cmd_info(char *args){ //print info
  if(args == NULL){
    printf("Unknown type!\n");
    return 0;
  }
  switch(*args){
    case 'r':
      for(int i = 0; i < 8; i++)
        printRegster(4, i);
      for(int i = 0; i < 8; i++)
        printRegster(2, i);
      for(int i = 0; i < 8; i++)
        printRegster(1, i);
      printRegster(-1, 0);
      break;
    case 'w':
      print_wp();
      break;
    default:
      printf("Unknown type!\n");
      break;
  }

  return 0;
}


static int cmd_p(char *args){ //caculation
  if(args == NULL){
    printf("Nothing to caculate!\n");
    return 0;
  }
  bool success = false;
  uint32_t res = expr(args, &success);
  if(!success)
    printf("Caculate failed!\n");
  else
    printf("%d\n", res);
  
  return 0;
}


static int cmd_x(char *args){ //scan memory
  if(args == NULL){
    printf("Nothing to scan!\n");
    return 0;
  }

  char *Size = strtok(args, " ");
  int size = -1;
  sscanf(Size, "%d", &size);
  if(size < 0){
    printf("Wrong size to scan!\n");
    return 0;
  }

  char *Addr = Size + strlen(Size) + 1;

  char *leftover;
  uint64_t addr = strtoul(Addr, &leftover, 0);
  if(*leftover != '\0'){
    printf("Wrong addr to scan!\n");
    return 0;
  }

  for(int i = 0; i < size; i++)
    printf("%#010X\t\n", vaddr_read(addr + i * 4, 4));

  return 0;
}


static int cmd_w(char *args){ //set watchpoint
  if(args == NULL){
    printf("Nothing to set!\n");
    return 0;
  }
  bool success = true;
  int64_t addr = expr(args, &success);
  if(!success){
    printf("Illigal expression to caculate!\n");
    return 0;
  }
  new_wp(addr, args);
  return 0;
}


static int cmd_d(char *args){ //delete watchpoiot
  if(args == NULL){
    printf("Nothing to delete!\n");
    return 0;
  }
  int n = atoi(args);
  free_wp(n);
  return 0;
} 


void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
