#ifndef __COMPUTATION_H__
#define __COMPUTATION_H__

#include "messages.h"


void changeWinSize(char change);
void computation_init(void);
void computation_cleanup(void);
bool is_computing(void);
bool is_done(void);
void abort_comp(void);
void enable_com(void);
bool is_abort(void);
bool is_set(void);
bool set_compute(message *msg);
bool compute(message *msg);
void update_data(const msg_compute_data *compute_data);
void get_grid_size(int *w, int *h);
void update_image(int w, int h, unsigned char *img);
bool calculateImg(message *output);
void clearBuffer(void);

#endif
