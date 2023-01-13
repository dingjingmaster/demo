#ifndef DUMP_WINDOW_H_incl
#define DUMP_WINDOW_H_incl 1

#include "test.priv.h"

extern int open_dump(const char *fn);
extern void close_dump(void);
extern void dump_window(WINDOW *w);

#endif /* DUMP_WINDOW_H_incl */
