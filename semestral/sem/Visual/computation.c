#include "computation.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "messages.h"
#include "event_queue.h"

static struct{
  double c_re;
  double c_im;
  int n;

  double range_re_min; 
  double range_re_max;
  double range_im_min; 
  double range_im_max;
  
  int grid_w;
  int grid_h;

  double cur_x; 
  double cur_y;
  uint8_t pos_x;
  uint8_t pos_y;

  double d_re; 
  double d_im;

  int nbr_chunks; 
  int cid; 
  double chunk_re; 
  double chunk_im;

  uint8_t chunk_n_re; 
  uint8_t chunk_n_im;

  uint8_t *grid;
  bool computing;
  bool abort;
  bool done;
  bool set;
}comp;

bool is_computing(void){
  return comp.computing;
}

bool is_done(void){
  return comp.done;
}

void abort_comp(void){
  comp.abort=true;
  comp.computing=false;
}

void enable_com(void){
  comp.abort=false;
}

bool is_abort(void){
  return comp.abort;
}

bool is_set(void){
  return comp.set;
}

bool set_recieved(const message *msg){
  my_assert(msg!=NULL,__func__,__LINE__,__FILE__);
  bool ret=! is_computing();
  if(ret){
    comp.c_re=msg->data.set_compute.c_re;
    comp.c_im=msg->data.set_compute.c_im;
    comp.d_re=msg->data.set_compute.d_re;
    comp.d_im=msg->data.set_compute.d_im;
    comp.n=msg->data.set_compute.n;
    comp.done=false;
    comp.set=true;
  }
  return ret;
}

void set_compute(const message *input){
  my_assert(input!=NULL,__func__,__LINE__,__FILE__);
  comp.cur_x=input->data.compute.re;
  comp.cur_y=input->data.compute.im;  
  comp.chunk_re=input->data.compute.re;
  comp.chunk_im=input->data.compute.im;
  comp.chunk_n_re=input->data.compute.n_re;
  comp.chunk_n_im=input->data.compute.n_im;
  comp.cid=input->data.compute.cid;
  comp.pos_x=0;
  comp.pos_y=0;
  comp.set=true;
}

bool calculateImg(message *output) {
  uint8_t iter = 0;
  
  double zx = comp.chunk_re + (comp.pos_x * comp.d_re);
  double zy = comp.chunk_im + (comp.pos_y * comp.d_im);
  double new_re=0.;
  double new_im=0.;

  while(iter <= comp.n){
    if(is_abort() || is_quit())
      return false;
    
    if(zx*zx + zy*zy < 4)
      iter+=1;
    else
      break;
    
    new_re=zx*zx - zy*zy + comp.c_re;
    new_im=2*zx*zy + comp.c_im;

    zx=new_re;
    zy=new_im;
  }

  output->data.compute_data.cid = comp.cid;
  output->data.compute_data.i_re = comp.pos_x;
  output->data.compute_data.i_im = comp.pos_y;
  output->data.compute_data.iter = iter;
  output->type = MSG_COMPUTE_DATA;

  comp.pos_x+= 1;
  if(comp.pos_x > comp.chunk_n_re) {
    comp.pos_x = 0;
    comp.pos_y+= 1;
    if(comp.pos_y > comp.chunk_n_im){
      return false;
    }
  }
  return true;
}

void get_grid_size(int *w, int *h){
  *w=comp.grid_w;
  *h=comp.grid_h;
}
