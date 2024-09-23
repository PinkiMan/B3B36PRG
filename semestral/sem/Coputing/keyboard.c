
#include "keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include "event_queue.h"
#include "utils.h"
#include "computation.h"


void* keyboard_thread(void* d){
  debug("keyboard_thread - start");

  event ev;
  int character;
  ev.type=EV_SERIAL;
  queue_push(ev);
  
  call_termios(0);
  while(! is_quit()){
    character=getchar();
    ev.type=EV_TYPE_NUM;
    switch(character){
      case 'q': //quit
        ev.type=EV_QUIT;
        set_quit();
        break;
      case 'g': //get version
        ev.type=EV_GET_VERSION;
        break;
      case 's': //set compute
        ev.type=EV_SET_COMPUTE;
        break;
      case '1': //starts calculation external
        ev.type=EV_COMPUTE;
        enable_com();
        break;
      case 'a': //aborts calculation
        ev.type=EV_ABORT;
        abort_comp();
        break;
      case 'r': //reset cid
        ev.type=EV_RESET_CHUNK;
        break;
      case 'd': //delete calculation
        ev.type=EV_CLEAR_BUFFER;
        break;
      case 'p': //edraw window
        ev.type=EV_REFRESH;
        break;
      case 'c': //compute local
        ev.type=EV_COMPUTE_CPU;
        break;
      case 'h': //help 
        printHelpMenu();
        break;
      case 'v': //save image
        ev.type=EV_SAVE_IMAGE;
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

