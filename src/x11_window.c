#include "x11_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "ll.h"

ll x11_window_ll

x11_window * x11_window_new(Display *display, int x_pos, int y_pos, Window parent_window, unsigned long event_mask, unsigned char win_background_color, int width, int height, int screen_number)
{

	/* Init variables */
	unsigned long window_attributes_mask;
	XSetWindowAttributes window_attributes;

	/*  Check for errors */
    if(x11_window_arr_index == MAX_WINDOWS) {
        fprintf(stderr, "Cannot create another window, at max: %d\n", MAX_WINDOWS);
        exit(EXIT_FAILURE);
    }

    /* Set window attributes mask */
	window_attributes_mask = CWBackPixel | CWEventMask;
	/* Set window attributes */
	/* Select events to listen on window */
	window_attributes.event_mask = event_mask;
	/* Set window background */
	if(win_background_color == X11_WHITE_WINDOW_BACKGROUND) {
		window_attributes.background_pixel = WhitePixel(display, screen_number);
	} else if (win_background_color == X11_BLACK_WINDOW_BACKGROUND) {
		window_attributes.background_pixel = BlackPixel(display, screen_number);
	} else {
		fprintf(stderr, "No valid window background color specified for window index: %u\n", x11_window_arr_index + 1);
		exit(EXIT_FAILURE);
	}

	/* Create main window */
	x11_window_arr[x11_window_arr_index].window = XCreateWindow(
			display,
			parent_window,
			x_pos,
			y_pos,
			width,
			height,
			1,
			XDefaultDepth(display,screen_number),
			InputOutput,
			XDefaultVisual(display,screen_number),
			window_attributes_mask,
			&window_attributes
			);

	x11_window_arr[x11_window_arr_index].height = height;
	x11_window_arr[x11_window_arr_index].width = width;
	x11_window_arr[x11_window_arr_index].screen_number = screen_number;



    x11_window_arr_index++;
    return &x11_window_arr[x11_window_arr_index - 1];
}

void x11_window_map_display(x11_window *self, Display *display) {
	/* Map main window to display */
	XMapWindow(display, self->window);
}
