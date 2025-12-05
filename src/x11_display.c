#include "util.h"
#include "x11_display.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

struct x11_display_struct x11_display;

void x11_display_init()
{
    /* Init variables */
    int num_monitors;
    int num_monitors_again;
    int default_monitor_number;
    XineramaScreenInfo *xin_monitors;

    /* Open connection to X server */
    x11_display.display = XOpenDisplay(NULL);
    if(x11_display.display == NULL)
        util_error_exit("Cannot connect to X server");

    /* Make sure at least one screen is connected */
    if((num_monitors = XScreenCount(x11_display.display)) < 1)
        util_error_exit("No monitors connected");

    /* See if multi-monitor support is enabled */
    xin_monitors = XineramaQueryScreens(x11_display.display, &num_monitors_again);

    /* If multi-monitor support not enabled, then set num_monitors to 1 and just let monitors point to default screen */
    if(xin_monitors == NULL) {
        default_monitor_number = XDefaultScreen(x11_display.display);
        if(default_monitor_number < 0)
            util_error_exit("Invalid default screen number");
        x11_display.monitors = x11_monitor_new(default_monitor_number);
        x11_display.num_monitors = 1;
        return;
    }

    /* TODO Else multi-monitor support enabled */

}

we
faw
ef
aw
ef
aw
ef
a
wef
aw
ef
a
we
f
awe
fa
we
fwa















