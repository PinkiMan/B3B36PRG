/*
 * Filename: event_queue.h
 * Date:     2017/04/15 12:41
 * Author:   Jan Faigl
 */

#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#include "messages.h"

typedef enum{
  EV_COMPUTE, // request compute on nucleo with particular 
  EV_RESET_CHUNK, //reset the chunk id
  EV_ABORT,
  EV_GET_VERSION,
  EV_SEND_VERSION,
  EV_THREAD_EXIT,
  EV_QUIT,
  EV_SERIAL,
  EV_SET_COMPUTE,
  EV_COMPUTE_CPU,
  EV_CLEAR_BUFFER,
  EV_REFRESH,
  EV_MSG_IN_PIPE,
  EV_SAVE_IMAGE,
  EV_ERROR,
  EV_TYPE_NUM
}event_type;

typedef struct event{
  //event_source source;
  event_type type;
  union{
    int param;
    message *msg;
  }data;
}event;

void queue_init(void);
void queue_cleanup(void);

event queue_pop(void);

void queue_push(event ev);

void set_quit();
bool is_quit();

#endif

/* end of event_queue.h */
