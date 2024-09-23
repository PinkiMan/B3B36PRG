#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "main.h" 
#include "keyboard.h" 
#include "gui.h" 

#include "event_queue.h"
#include "utils.h"
#include "prg_io_nonblock.h"

#ifndef IO_READ_TIMEOUT_MS
#define IO_READ_TIMEOUT_MS 100
#endif


void* read_pipe_thread(void *d);

int main(const int argc, const char *argv[]){
  int pipe_in,pipe_out;

  const char *fname_pipe_in= (argc>1)? argv[1] : "/tmp/computational_module.out";
  const char *fname_pipe_out= (argc>2)? argv[2] : "/tmp/computational_module.in";

  

  pipe_in=io_open_read(fname_pipe_in);
  my_assert(pipe_in!=-1,__func__,__LINE__,__FILE__);
  pipe_out=io_open_write(fname_pipe_out);
  my_assert(pipe_out!=-1,__func__,__LINE__,__FILE__);



  enum{MAIN_THRD,READ_PIPE_THRD,KEYBOARD_THRD,WIN_THRD, NUM_THRDS};
  const char *thrd_names[]={"Main","ReadPipe","Keyboard","Window"};
  void* (*thrd_functions[])(void*)={mainThread,read_pipe_thread,keyboard_thread,gui_win_thread};
  pthread_t threads[NUM_THRDS];
  void* thrd_data[NUM_THRDS]={};
  thrd_data[READ_PIPE_THRD]=&pipe_in;
  thrd_data[MAIN_THRD]=&pipe_out;

  for(size_t i=0;i<NUM_THRDS;++i){
    int r=pthread_create(&threads[i], NULL, thrd_functions[i], thrd_data[i]);
    printf("Created thread '%s' %s\r\n",thrd_names[i],( (r==0)? "OK":"FAIL") );
  }
  
  int *ex;
  for(size_t i=0;i<NUM_THRDS;++i){
    int r=pthread_join(threads[i], (void*)&ex);
    printf("Joining the thread %s has been %s\r\n", thrd_names[i],( (r==0)? "OK":"FAIL"));
  }

  io_close(pipe_in);
  io_close(pipe_out);
  return EXIT_SUCCESS;
}

void* read_pipe_thread(void *d){
  debug("read_pipe_thread - start");
  my_assert(d!=NULL,__func__,__LINE__,__FILE__);
  int pipe_in= *(int*)d;
  uint8_t msg_buf[sizeof(message)];
  size_t idx=0;
  size_t len=0;

  unsigned char c;
  while(io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c) > 0); 

  while(! is_quit()){
    int r= io_getc_timeout(pipe_in,IO_READ_TIMEOUT_MS, &c);
    if(r>0){ 
      if(idx==0){
        len=0; 
        if(get_message_size(c, &len))
          msg_buf[idx++]=c;
        else
          fprintf(stderr,"ERROR: unknown message type 0x%x!\n",c);
      }else 
        msg_buf[idx++]=c;
      
      if(len!=0 && idx==len){ 
        message *msg=my_alloc(sizeof(message));
        if(parse_message_buf(msg_buf,len,msg)){
          event ev = {.type=EV_MSG_IN_PIPE};
          ev.data.msg=msg;
          queue_push(ev);
        }else{
          fprintf(stderr,"ERROR: cannot parse message type %d\n",msg_buf[0]);
          free(msg);
        }
        idx=len=0;
      }

    }else if(r==0){ //timeout

    }else{ 
      error("read from pipe");
      set_quit();
      event ev={.type=EV_QUIT};
      queue_push(ev);
    }
  }
  debug("read_pipe_thread - end");
  return NULL;
}


