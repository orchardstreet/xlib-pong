#ifndef GLOBALS_H
#define GLOBALS_H
#include <X11/Xlib.h>
#include "x11_display.h"

typedef struct context_struct {

        x11_display *main_display;

} context;

enum exit_codes{SUCCESS, FAILURE, EXIT_PROGRAM};

extern context game_context;

#endif
