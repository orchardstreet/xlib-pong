#ifndef x11_window_h
#define x11_window_h
#define MAX_WINDOWS 200
#include <X11/Xlib.h>
enum x11_window_background_colors_enum {X11_WHITE_WINDOW_BACKGROUND, X11_BLACK_WINDOW_BACKGROUND};

typedef struct x11_window_struct {
    Window window;
    int width;
    int height;
    int screen_number;
} x11_window;

x11_window * x11_window_new(Display *display, int x_pos, int y_pos, Window parent_window, unsigned long event_mask, unsigned char win_background_color, int width, int height, int screen_number);

#endif
