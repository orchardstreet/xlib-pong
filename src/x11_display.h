#ifndef X11_DISPLAY_H
#define X11_DISPLAY_H
#include <inttypes.h>
#include <X11/Xlib.h>
#include "x11_monitor.h"

struct x11_display_struct {
    Display *display;
    uint32_t num_monitors;
    x11_monitor * monitors; /* initialize with malloc */
    uint8_t has_multi_monitor_support;
};

extern struct x11_display_struct x11_display;

void x11_display_init(void);

#endif
