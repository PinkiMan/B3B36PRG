#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>

#include "messages.h"

void my_assert(_Bool r, const char *fcname, int line, const char *fname);
void* my_alloc(size_t size);
void call_termios(_Bool reset);
void set_version(message *msg);
void printHelpMenu(void);
void print_module_init(void);

void info(const char *str);
void debug(const char *str);
void warn(const char *str);
void error(const char *str);

#endif
