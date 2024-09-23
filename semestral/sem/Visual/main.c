
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include "event_queue.h"
#include "utils.h"
#include "messages.h"
#include "computation.h"

static void processMsgPipe(event *const ev);

void* mainThread(void* d){
  debug("mainThread - start");
  my_assert(d!=NULL,__func__,__LINE__,__FILE__);
  int pipe_in= *(int*)d;
  message msg;
  uint8_t msg_buf[sizeof(message)];
  size_t msg_len;

  while(! is_quit()){
    event ev=queue_pop();
    msg.type=MSG_NBR;
    switch(ev.type){
      case EV_ABORT:
        debug("ABORT");
        msg.type=MSG_ABORT;
        abort_comp();
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
      case EV_MSG_IN_PIPE:
        processMsgPipe(&ev);
        break;
      case EV_OK:
        msg.type=MSG_OK;
        break;
      case EV_ERROR:
        msg.type=MSG_ERROR;
        break;
      case EV_SEND_DONE:
        msg.type=MSG_DONE;
        break;
      case EV_SEND_DATA:
        debug("SEND");
        event ev2;
        enable_com();
        if(is_abort())
          break;
        if(calculateImg(&(msg))){
          ev2.type=EV_SEND_DATA;
          queue_push(ev2);
        }else{ 
          ev2.type=EV_SEND_DONE;
          queue_push(ev2);
        }
        break;
      default:
        fprintf(stderr,"Unhandled event type %d\n",ev.type);
        break;
    }
    if(msg.type!=MSG_NBR){
      my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len) ,__func__,__LINE__,__FILE__);
      write(pipe_in,msg_buf,msg_len);
    }

  }
 
  debug("mainThread - end");
  return NULL;
}

static void processMsgPipe(event *const ev){
  my_assert(ev != NULL && ev->type==EV_MSG_IN_PIPE && ev->data.msg!=NULL,__func__,__LINE__,__FILE__);
  ev->type=EV_TYPE_NUM;
  const message *msg=ev->data.msg;
  const msg_version version=msg->data.version;
  event ev2;
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
      ev2.type=EV_OK;
      queue_push(ev2);
      break;
    case MSG_GET_VERSION:
      ev2.type=EV_SEND_VERSION;
      queue_push(ev2);
      break;
    case MSG_VERSION:
      fprintf(stderr,"INFO: Version %d.%d-p%d\n",version.major,version.minor,version.patch);
      break;
    
    case MSG_SET_COMPUTE: 
      debug("SET_COMPUTE");
      if(set_recieved(msg)){
        ev2.type=EV_OK;
        enable_com();
      }else
        ev2.type=EV_ERROR;
      queue_push(ev2);
      break;
    case MSG_COMPUTE: 
      debug("MSG_COMPUTE");
      set_compute(msg);
      ev2.type= (! is_set())? EV_ERROR : EV_SEND_DATA;
      queue_push(ev2);
      break;

    default:
      fprintf(stderr,"Unhandled pipe message type %d\n",msg->type);
      break;
  }
  free(ev->data.msg);
  ev->data.msg=NULL;
}
