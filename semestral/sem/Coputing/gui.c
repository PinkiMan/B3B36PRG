

#include "gui.h"
#include <SDL.h>
#include "xwin_sdl.h"
#include "utils.h"
#include "computation.h"
#include "event_queue.h"

#ifndef SDL_EVENT_POLL_WAIT_MS
#define SDL_EVENT_POLL_WAIT_MS 10
#endif

static void keyDownMenu(const SDL_Event event);

extern SDL_Window *win;

static struct{
  int w;
  int h;
  unsigned char *img;
}gui={
  .img=NULL
};

void* gui_win_thread(void *d){
  debug("gui_win_thread - start");
  SDL_Event event;
  struct event ev;
  while(! is_quit()){
    ev.type=EV_TYPE_NUM;
    if(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_KEYDOWN:
          keyDownMenu(event);
          break;
        case SDL_KEYUP:
          break;
        case SDL_MOUSEMOTION:
          break;
        default:
          break;
      }
    }
    SDL_Delay(SDL_EVENT_POLL_WAIT_MS);
    if(ev.type!=EV_TYPE_NUM)
      queue_push(ev);
  }

  debug("gui_win_thread - end");
  return NULL;
}

static void keyDownMenu(const SDL_Event event){
  struct event ev;
  ev.type=EV_TYPE_NUM;
  switch(event.key.keysym.sym){
    case SDLK_q: //quits
      ev.type=EV_QUIT;
      break;
    case SDLK_g: //version
      ev.type=EV_GET_VERSION;
      break;
    case SDLK_s: //set parameters
      ev.type=EV_SET_COMPUTE;
      break; 
    case SDLK_1: //start calculation
      ev.type=EV_COMPUTE;
      enable_com();
      break;
    case SDLK_a: //aborts calculation
      ev.type=EV_ABORT;
      abort_comp();
      break;
    case SDLK_r: //resets cid
      ev.type=EV_RESET_CHUNK;
      break;
    case SDLK_d: //delete calculation
      ev.type=EV_CLEAR_BUFFER;
      break;
    case SDLK_p: //redraws window
      ev.type=EV_REFRESH;
      break;
    case SDLK_c: //computes local
      ev.type=EV_COMPUTE_CPU;
      break;
    case SDLK_h: //help
      printHelpMenu();
      break;
    case SDLK_v: //save image
      ev.type=EV_SAVE_IMAGE;
  
    default:
      info("Unhandled keyboard input!");
      break;
  }
  if(ev.type!=EV_TYPE_NUM)
    queue_push(ev);
}

void gui_init(void){
  get_grid_size(&gui.w, &gui.h); 
  gui.img=my_alloc(gui.w * gui.h *3);
  my_assert(xwin_init(gui.w,gui.h)==0, __func__,__LINE__,__FILE__);
}

void gui_cleanup(void){
  if(gui.img){
    free(gui.img);
    gui.img=NULL;
  }
  xwin_close();
}

void gui_refresh(void){
  if(gui.img){
    update_image(gui.w,gui.h,gui.img);
    xwin_redraw(gui.w, gui.h, gui.img);
  }
}

