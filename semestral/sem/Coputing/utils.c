#include "utils.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h> 

void my_assert(bool r, const char *fcname, int line, const char *fname){
  if(! r){
    fprintf(stderr,"ERROR: %s failed: %s() line %d in %s\n",__func__,fcname,line,fname);
    exit(105);
  }
}

void *my_alloc(size_t size){
  void *ret=malloc(size);
  if(! ret){
    fprintf(stderr,"ERROR: %s Malloc not allocated!\n",__func__);
    exit(200);
  }
  return ret;
}

void call_termios(bool reset){
  static struct termios tio, tioOld;
  tcgetattr(STDIN_FILENO, &tio);
  if(reset){
    tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
  }else{
    tioOld = tio; 
    cfmakeraw(&tio);
    tio.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &tio);
  }
}

void printHelpMenu(void){
  printf("\n--- Help Menu ---\n");
  printf("q: Exits application\n");
  printf("g: Gets version number\n");
  printf("s: Sends parameters to the computing module\n");
  printf("c: Starts calcuation on local module\n"); 
  printf("1: Starts calcuation on computation module\n");
  printf("a: Aborts the computation\n");
  printf("r: Resets ID of chunk\n");
  printf("d: Delete calculation progress\n");
  printf("p: Redraws the window\n");
  printf("h: Prints help menu\n");
  printf("v: Saves state to PNG image\n");
}

void set_version(message *msg){
  msg->type=MSG_VERSION;
  msg->data.version.major=21;
  msg->data.version.minor=05;
  msg->data.version.patch=17;
}

void print_module_init(void){
  printf("\n\n-----------------------------\n");
  printf("   Press 'h' for Help Menu\n\n");
}




void info(const char *str)
{
    fprintf(stderr, "INFO: %s\n",str);
}
void debug(const char *str)
{
    fprintf(stderr, "DEBUG: %s\n",str);
}
void warn(const char *str)
{
    fprintf(stderr, "WARNNING: %s\n",str);
}
void error(const char *str)
{
    fprintf(stderr, "ERROR: %s\n",str);
}
