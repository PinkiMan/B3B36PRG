#ifndef __COMPUTATION_H__
#define __COMPUTATION_H__

#include "messages.h"

bool is_computing(void);
bool is_done(void);
void abort_comp(void);
void enable_com(void);
bool is_abort(void);
bool is_set(void);

bool set_recieved(const message *msg);
void set_compute(const message *input);
bool calculateImg(message *output);
void get_grid_size(int *w, int *h);

#endif
