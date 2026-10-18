#ifndef X11_DISPLAY_H
#define X11_DISPLAY_H
#include <inttypes.h>
#include <limits.h>
#include <X11/Xlib.h>
#include "x11_screen.h"
#include "ll.h"
#define MAX_DISPLAYS INT_MAX

typedef struct x11_display_struct {
	struct x11_display_struct *next;
	struct x11_display_struct *prior;
    Display *display;
	ll screen_ll; /* List of attached screens */	
	int default_screen_number;
} x11_display;

uint8_t x11_display_new(x11_display **display_return, char *display_env);

#endif
