#include "computation.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "messages.h"
#include "event_queue.h"
#include "gui.h"


static struct{
  double c_re;
  double c_im;
  size_t n; // number of iterations for pixel

  double range_re_min; //real
  double range_re_max;
  double range_im_min; //imaginary
  double range_im_max;
  
  size_t grid_w;
  size_t grid_h;

  int cur_x; //actual spot
  int cur_y;

  double d_re; //density of spot
  double d_im;

  size_t nbr_chunks; //number of chunks 
  size_t cid;   //chunk's id
  double chunk_re; //coords
  double chunk_im;

  size_t chunk_n_re; //number of chunk's spots
  size_t chunk_n_im;

  uint8_t *grid;
  bool computing;
  bool abort;
  bool done;
  bool set;

  uint8_t pos_x;
  uint8_t pos_y;
  uint8_t actual_grid;
}comp={
  .c_re=-0.4,
  .c_im=0.6,
  .n=60,

  .range_re_min=-1.6,
  .range_re_max=1.6,
  .range_im_min=-1.1,
  .range_im_max=1.1,

  .grid=NULL,

  .computing=false,
  .abort=false,
  .done=false,
  .set=false,

  .pos_x=0,
  .pos_y=0,
  .actual_grid=0,
};

size_t Large_Resolution[]={3840,2160};
size_t Normal_Resolution[]={1920,1080};
size_t Small_Resolution[]={640,480};





void changeWinSize(char change){
  comp.actual_grid=change;

  size_t *Resolution;
  switch(change){
    case 0:
      Resolution=Small_Resolution;
      break;
    case 1:
      Resolution=Normal_Resolution;
      break;
    case 2:
      Resolution=Large_Resolution;
      break;
    default:
      Resolution=Small_Resolution;
      break;
  }

  comp.grid_w=Resolution[0];
  comp.grid_h=Resolution[1];
  comp.chunk_n_re=comp.grid_w/10;
  comp.chunk_n_im=comp.grid_h/10;
}

void computation_init(void){
  printf("Resolution is %ld x %ld\n",comp.grid_w,comp.grid_h);
  comp.grid=my_alloc(comp.grid_w * comp.grid_h);
  comp.d_re=(comp.range_re_max - comp.range_re_min) / (1. * comp.grid_w);
  comp.d_im=-(comp.range_im_max - comp.range_im_min) / (1. * comp.grid_h);
  comp.nbr_chunks=(comp.grid_h * comp.grid_w) / (1.*(comp.chunk_n_re * comp.chunk_n_im ));
  comp.cur_x=0;
  comp.cur_y=0;
  comp.cid=0;
  comp.computing=false;
  comp.done=0;
  comp.abort=0;
  
}

void computation_cleanup(void){
  if(comp.grid){
    free(comp.grid);
  }
  comp.grid=NULL;
}

bool is_computing(void){
  return comp.computing;
}

bool is_done(void){
  return comp.done;
}

void abort_comp(void){
  comp.abort=!comp.abort;
}

void enable_com(void){
  if(!is_computing()){
    comp.abort=false;
  }
}

bool is_abort(void){
  return comp.abort;
}

bool is_set(void){
  return comp.set;
}

bool set_compute(message *msg){
  my_assert(msg!=NULL,__func__,__LINE__,__FILE__);
  bool ret=!is_computing();
  if(ret){
    msg->type=MSG_SET_COMPUTE;
    msg->data.set_compute.c_re=comp.c_re;
    msg->data.set_compute.c_im=comp.c_im;
    msg->data.set_compute.d_re=comp.d_re;
    msg->data.set_compute.d_im=comp.d_im;
    msg->data.set_compute.n=comp.n;
    comp.done=false;
    comp.set=true;
  }else{
    warn("During computing data would be discarted!");
  }
  return ret;
}

bool compute(message *msg){
  my_assert(msg!=NULL,__func__,__LINE__,__FILE__);
  if(! is_computing()){
    comp.cid=0;
    comp.computing=1;
    comp.cur_y=0;
    comp.cur_x=0; 
    comp.chunk_im=comp.range_im_max; //(0,0)
    comp.chunk_re=comp.range_re_min; 
    msg->type=MSG_COMPUTE;

  }else{ 
    comp.cid+=1;
    if(comp.cid<comp.nbr_chunks){
      comp.cur_x+=comp.chunk_n_re;
      comp.chunk_re+=comp.chunk_n_re * comp.d_re;
      if(comp.cur_x >= comp.grid_w){
        comp.cur_x=0;
        comp.chunk_re=comp.range_re_min;
        comp.chunk_im+=comp.chunk_n_im * comp.d_im;
        comp.cur_y+=comp.chunk_n_im;
      }
    }else{ 
      error("Chunk ID run out of array!");
    }
    msg->type=MSG_COMPUTE;
  }
  if(comp.computing && msg->type==MSG_COMPUTE){
    msg->data.compute.cid=comp.cid;
    msg->data.compute.re=comp.chunk_re;
    msg->data.compute.im=comp.chunk_im;
    msg->data.compute.n_re=comp.chunk_n_re;
    msg->data.compute.n_im=comp.chunk_n_im;
  }
  return is_computing();
}

void update_data(const msg_compute_data *compute_data){
  my_assert(compute_data!=NULL,__func__,__LINE__,__FILE__);
  if(compute_data->cid==comp.cid){
    const int idx=comp.cur_x + compute_data->i_re + (comp.cur_y + compute_data->i_im) * comp.grid_w;
    if(idx>=0 && idx<(comp.grid_w * comp.grid_h))
      comp.grid[idx]=compute_data->iter;
    if((comp.cid +1) >=comp.nbr_chunks && (compute_data->i_re +1)==comp.chunk_n_re && (compute_data->i_im +1)==comp.chunk_n_im){
      comp.done=true;
      comp.computing=false;
      info("--- Compute finished ---");
    }
  }else
    warn("Bad chunk ID");
}

void get_grid_size(int *w, int *h){
  *w=comp.grid_w;
  *h=comp.grid_h;
}

void update_image(int w,int h,unsigned char *img){
  my_assert(img && comp.grid && (w==comp.grid_w) && (h==comp.grid_h),__func__,__LINE__,__FILE__);
  for(size_t i=0;i<w*h;++i){
    const double t=1. * comp.grid[i] / (comp.n +1.);
    *(img++)= 9*(1-t)*t*t*t*255;
    *(img++)=15*(1-t)*(1-t)*t*t*255;
    *(img++)=8.5*(1-t)*(1-t)*(1-t)*t*255;
  }
}

bool calculateImg(message *output){
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
      comp.pos_y=0;
      return false; 
    } 
  }
  return true; 
}

void clearBuffer(void){
  abort_comp();
  gui_refresh();
  computation_cleanup();
  computation_init();
}
