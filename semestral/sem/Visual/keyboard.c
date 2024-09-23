

#include "keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include "event_queue.h"
#include "utils.h"

void* keyboard_thread(void* d){
  debug("keyboard_thread - start");

  call_termios(0);
  event ev;
  ev.type=EV_SERIAL;
  queue_push(ev);
  int c;
  while(! is_quit()){
    c=getchar();
    ev.type=EV_TYPE_NUM;
    switch(c){
      case 'q': //quit
        ev.type=EV_QUIT;
        set_quit();
        break;
      case 'g': //get version
        ev.type=EV_GET_VERSION;
        break;
      case 'a': //aborts calculation
        ev.type=EV_ABORT;
        break;
      case 'h': //help
        printHelpMenu();
        break;   
      default:
        debug("Unhandled keyboard input!");
        break; 
    }
    if(ev.type!=EV_TYPE_NUM)
      queue_push(ev);    
  }

  call_termios(1); //restores without raw
  debug("keyboard_thread - end");
  return NULL;
}
