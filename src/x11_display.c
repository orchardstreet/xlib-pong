#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "x11_display.h"
#include "ll.h"
#include "x11_screen.h"
#include "globals.h"

#define MAX_SCREENS_PER_DISPLAY 100

ll x11_display_ll; /* Display linked list */
/* Easier and slower head access, eg *x11_display_ll_head.display or *x11_display_ll_head->prior */
x11_display **x11_display_ll_head = (x11_display **) &(x11_display_ll.head);
/* Easier and slower tail access, eg *x11_display_ll_tail.display or *x11_display_ll_tail->next */
x11_display **x11_display_ll_tail = (x11_display **) &(x11_display_ll.tail);


uint8_t 
x11_display_new(x11_display **display_return, char *display_env) {
/* Connect to an X11 server at display address in display_env
 * Fill out a an x11_display struct with display details and add it to 
 * this module's display linked list.
 * 
 * Include all screens of the display in a screen linked list within the
 * new display struct/node. 
 *
 * Function returns EXIT_PROGRAM on fatal error, FAILURE on possibly 
 * recoverable in main() and SUCCESS on success */

    /* Init variables */
    x11_display *new_display;
    Display *display;
    int num_screens; /* Number of screens in display */
    int default_screen_number; /* The default screen of display */
	x11_screen *screen_ptr; /* Pointer to screen struct */
	ll *screen_ll; /* Screen linked list */
	int i;

    /* Connect to X server at display_env address */
    display = XOpenDisplay(display_env);
    if(display == NULL) {
        fprintf(stderr,"Cannot connect to X server at specified display address\n");
        return FAILURE; 
    }

    /* Get number of screens of the display and make sure at least one screen is connected
     * but no more than max screens per display allowed */
    if((num_screens = XScreenCount(display)) < 1) {
       fprintf(stderr,"No screens connected\n"); 
       return FAILURE;
    }
	if(num_screens > MAX_SCREENS_PER_DISPLAY) {
		fprintf(stderr,"%d is too many screens per display\n", MAX_SCREENS_PER_DISPLAY);
		return FAILURE;
	}

	/* Get the default screen number of the display */
    default_screen_number = XDefaultScreen(display);
    if(default_screen_number < 0) {
        fprintf(stderr,"Invalid default screen number\n");
        return FAILURE;
    }

    /* Append new blank display node to display linked list */
    if(ll_insert_blank(&x11_display_ll, 0, sizeof(x11_display),\
	    MAX_DISPLAYS, (ll_node**)&new_display) == EXIT_PROGRAM) {
        return EXIT_PROGRAM;
	}

	/* Fill out blank display linked list node */
	new_display->display = display;
	new_display->default_screen_number = default_screen_number;

    /* Initialize all screens of display */
	screen_ll = &(new_display->screen_ll);
	for(i=0; i < num_screens; i++) {
		if(ll_insert_blank(screen_ll, 0, sizeof(x11_screen),\
		    MAX_SCREENS_PER_DISPLAY, (ll_node**)&screen_ptr) == EXIT_PROGRAM) {
			return EXIT_PROGRAM;
		}
		x11_screen_new(screen_ptr);
	}

    /* Success, new display created, return node to user */
    *display_return = new_display;
    return SUCCESS;
}
