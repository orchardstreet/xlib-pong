#ifndef X11_SCREEN_H
#define X11_SCREEN_H
#include <inttypes.h>

typedef struct x11_screen_struct {
	struct x11_screen_struct *next;
	struct x11_screen_struct *prior;
    uint32_t width;
    uint32_t height;
    int screen_number;
} x11_screen;

uint8_t x11_screen_new(x11_screen *new_screen);

#endif
