
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

#include "event_queue.h"
#include "utils.h"
#include "messages.h"
#include "computation.h"
#include "gui.h"
#include "xwin_sdl.h"

static void processMsgPipe(event *const ev);
static bool processResolution(void);

void* mainThread(void* d){
  debug("mainThread - started");
  my_assert(d!=NULL,__func__,__LINE__,__FILE__);
  int pipe_out= *(int*)d;
  bool problem=processResolution();

  message msg;
  uint8_t msg_buf[sizeof(message)];
  size_t msg_len;
 
  //initialize computation & visualization
  if(! is_quit() && ! problem){
    computation_init();
    gui_init();
  }
  while(! is_quit() && !problem){
    event ev=queue_pop();
    msg.type=MSG_NBR;
    switch(ev.type){
      case EV_COMPUTE:
        if(! is_abort()) info(compute(&msg)? "compute OK":"compute FAIL");
        break;
      case EV_RESET_CHUNK:
        gui_refresh();
        computation_init();
        info("Chunk-ID reset");
        break;
      case EV_ABORT:
        debug("ABORT");
        msg.type=MSG_ABORT;
        break;
      case EV_GET_VERSION:
        debug("GET_VERSION");
        msg.type=MSG_GET_VERSION;
        break;
      case EV_SEND_VERSION:
        debug("VERSION");
        set_version(&msg);
        break;
      case EV_QUIT:
        debug("QUIT");
        set_quit();
        break;
      case EV_SERIAL:
        print_module_init();
        break;
      case EV_CLEAR_BUFFER:
        clearBuffer();
        msg.type=MSG_ABORT;
        break;
      case EV_SET_COMPUTE:
        debug("SET_COMPUTE");
        if(! is_abort())
          info(set_compute(&msg)? "set compute OK":"set compute FAIL");
        break;
      case EV_COMPUTE_CPU:
        debug("COMPUTE_CPU");
        message msg2;
        if(! is_abort()) info(compute(&msg)? "compute OK":"compute FAIL");
        while(calculateImg(&msg2) && (!is_quit())) {
          if(! is_abort()) update_data(&(msg2.data.compute_data));
        }
        gui_refresh();
        if(! is_done()){
          event ev2={.type=EV_COMPUTE_CPU};
          queue_push(ev2);
        }
        msg.type=MSG_NBR;
        msg2.type=MSG_NBR;
        break;
      case EV_REFRESH: //forcely redraws window
        gui_refresh();
        info("Window redrawed from buffer");
        break;
      case EV_MSG_IN_PIPE:
        processMsgPipe(&ev);
        break;
      case EV_SAVE_IMAGE:
        xwin_save_image();
        break;
      default:
        fprintf(stderr,"Unhandled event type %d\n",ev.type);
        break;
    }
    if(msg.type!=MSG_NBR){
      my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len) ,__func__,__LINE__,__FILE__);
      if(! (write(pipe_out,msg_buf,msg_len)==msg_len))
        error("sendind data to pipe_out failed!");
    }

  }

  //cleanup computation & visualization
  if(! problem){
    computation_cleanup();
    gui_cleanup();
  }
  debug("mainThread - ended");
  return NULL;
}

static void processMsgPipe(event *const ev){
  my_assert(ev != NULL && ev->type==EV_MSG_IN_PIPE && ev->data.msg!=NULL,__func__,__LINE__,__FILE__);
  ev->type=EV_TYPE_NUM;
  const message *msg=ev->data.msg;
  const msg_version version=msg->data.version;
  switch(msg->type){
    case MSG_OK:
      info("MSG_OK");
      break;
    case MSG_ERROR:
      warn("MSG_ERROR");
      break;
    case MSG_ABORT:
      info("MSG_ABORT");
      abort_comp();
      break;
    case MSG_DONE:
      gui_refresh();
      if(! is_done()){
        event ev={.type=EV_COMPUTE};
        queue_push(ev);
      }
      break;
    case MSG_GET_VERSION:
      event ev={.type=EV_SEND_VERSION};
      queue_push(ev);
      break;
    case MSG_VERSION:
      fprintf(stderr,"\rINFO: Version. %d.%d-p%d\n",version.major,version.minor,version.patch);
      break;
    
    case MSG_COMPUTE_DATA:
      if(! is_abort()) update_data(&(msg->data.compute_data));
      break;

    default:
      fprintf(stderr,"Unhandled pipe message type %d\n",msg->type);
      break;
  }
  free(ev->data.msg);
  ev->data.msg=NULL;
}

static bool processResolution(void){
  call_termios(0);
  int c='.';
  int ret=0;
  do{
    c='.';
    printf("\n--- Choose resolution ---\n");
    printf("   0:  640 x 480\n   1: 1920 x 1080\n   2: 3840 x 2160\n\n");
    c=getchar();
    if(c=='q'){
      event ev={.type=EV_QUIT};
      queue_push(ev);
      set_quit();
      return 1;
    }else if(c=='0' || c=='1'|| c=='2'){
      ret=c-'0';
      info("Got valid resolution input");
    }else{
      info("Wrong resolution input");
    }
  }while((c<'0' || c >='3') && !is_quit());
  changeWinSize(ret);
  call_termios(1);
  return 0;
}
