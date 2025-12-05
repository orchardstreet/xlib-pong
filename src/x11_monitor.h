#ifndef X11_MONITOR_H
#define X11_MONITOR_H
#include <inttypes.h>

typedef struct x11_monitor_struct {
    uint32_t width;
    uint32_t height;
    int monitor_number;
} x11_monitor;

x11_monitor * x11_monitor_new(int num_new_monitors);

#endif
