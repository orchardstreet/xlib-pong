/* TODO error handling */
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#define MAX_WINDOWS 30
#define FRAME_DURATION 14000 /* in nanoseconds */

/* Global variables */
Display *display;
int screen_number;
XEvent event_x11;
enum window_background_colors_enum {WHITE_WINDOW_BACKGROUND, BLACK_WINDOW_BACKGROUND};
enum paddles_enum {LEFT_PADDLE, RIGHT_PADDLE};
enum direction_enum {NO_DIRECTION = 0, DIRECTION_UP = -1, DIRECTION_DOWN = 1};
enum direction_enum2 {DIRECTION_LEFT= -1, DIRECTION_RIGHT = 1};
enum user_enum {HUMAN, COMPUTER};
enum exit_codes_enum {EXIT_PROGRAM, SUCCESS, FAILURE};
int usleep(useconds_t usec);
float random_reaction = 30;

/* Struct that holds all windows in the program */
struct window_struct 
{
	Window window_number;
	int window_x_pos;
	int window_y_pos;
};

struct ball_struct
{
	float x_pos;
	float y_pos;
	signed char width;
	float velocity;
	float slope;
	struct window_struct *window;
	signed char passed;
};

struct paddle_struct
/* If switching from computer to user or visa versa, it is necessary to 0 out the direction and pressed variables 
 * unless adding separate variables here for each */
{
	struct window_struct *window;
	signed char direction;
	signed char up_button_pressed;
	signed char down_button_pressed;
	signed char user;
	signed char velocity;
	signed char default_velocity;
	signed char position_changed;
	signed char height;
	unsigned long score;
	int old_y_pos;
};

/* Function to exit program as error */
void 
error_exit(char *error_str) 
{
	fprintf(stderr,"%s\n",error_str);
	exit(EXIT_FAILURE);
}

/* Function to create a window and map it to the display */
void 
create_and_map_window(struct window_struct *windows, Window parent_window, int *num_windows, unsigned long event_mask,
		unsigned char background_color, int x_pos, int y_pos, int width, int height) 
{
	
	/* Init variables */
	Window window_number;
	XSetWindowAttributes window_attributes;
	unsigned long window_attributes_mask;

	/* Exit if about to exceed MAX_WINDOWS */
	if(*num_windows >= MAX_WINDOWS)
		error_exit("Number of windows created exceeds MAX_WINDOWS, exiting...");

	/* Set the window attributes */
	window_attributes_mask = CWBackPixel | CWEventMask;

	/* Set background color of main window */
	if(background_color == WHITE_WINDOW_BACKGROUND)
		window_attributes.background_pixel = WhitePixel(display,screen_number);
	if(background_color == BLACK_WINDOW_BACKGROUND)
		window_attributes.background_pixel = BlackPixel(display,screen_number);

	/* Select events to listen on window */
	window_attributes.event_mask = event_mask;

	/* Create main window */
	window_number = XCreateWindow(
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
	printf("Created window %lu\n",window_number);

	/* Map main window to display */
	XMapWindow(display,window_number);

	/* Save important window properties to a dedicated struct 
	 * in the windows array */
	windows[*num_windows].window_number = window_number;
	windows[*num_windows].window_x_pos = x_pos;
	windows[*num_windows].window_y_pos = y_pos;
	*num_windows = *num_windows + 1;

	/* Wait for mapnotify and configurenotify events associated with newly mapped windows */
	if(*num_windows == 1) {
		/*XWindowEvent(display,window_number, StructureNotifyMask, &event_x11);*/
		/*XWindowEvent(display,window_number, StructureNotifyMask, &event_x11);*/
		/*XWindowEvent(display,window_number, ExposureMask, &event_x11);*/
	} else {
		/*XWindowEvent(display,window_number, StructureNotifyMask, &event_x11);*/
	}
}

unsigned char
process_event_queue(int num_events, struct paddle_struct *paddles, Atom wmDeleteMessage)
{
	int event_number;
	KeySym keysym;
	//printf("Processing event queue...\n");
	for(event_number = 0; event_number < num_events; ++event_number) {
		//printf("Got event number %d: ",event_number);
		XNextEvent(display, &event_x11);
		switch(event_x11.type) {
			case ConfigureNotify: /* possibly useful if we resize window */
				printf("ConfigureNotify event on window: %lu\n",event_x11.xexpose.window);
				break;
			case Expose: /* possibly useful if we resize window */
				printf("Expose event on window: %lu\n",event_x11.xexpose.window);
				break;
			case ClientMessage: /* startup adds one of these */
				printf("ClientMessage event\n");
				if(event_x11.xclient.data.l[0] == wmDeleteMessage)
					return EXIT_PROGRAM;
				break;
			case MapNotify: /* No idea if this is useful */
				printf("MapNotify event on window: %lu\n",event_x11.xexpose.window);
				break;
			case KeyPress: /* Go to function from here */
				//printf("KeyPress event\n");
				keysym = XLookupKeysym(&event_x11.xkey,0);
				switch(keysym) {
					case XK_Up:
						//printf("Up arrow down event\n");
						/* Discard when repeat press or if computer is user */
						if(paddles[RIGHT_PADDLE].up_button_pressed == 1 || paddles[RIGHT_PADDLE].user == COMPUTER) 
							break;
						paddles[RIGHT_PADDLE].up_button_pressed = 1; /* Up arrow is pressed down */
						if (paddles[RIGHT_PADDLE].down_button_pressed) {
							paddles[RIGHT_PADDLE].direction = NO_DIRECTION;
						} else {
							paddles[RIGHT_PADDLE].direction = DIRECTION_UP;
						}
						break;
					case XK_Down:
						//printf("Down arrow down event\n");
						/* Discard when repeat press or if computer is user */
						if(paddles[RIGHT_PADDLE].down_button_pressed == 1 || paddles[RIGHT_PADDLE].user == COMPUTER) 
							break;
						paddles[RIGHT_PADDLE].down_button_pressed = 1; /* Up arrow is pressed down */
						if (paddles[RIGHT_PADDLE].up_button_pressed) {
							paddles[RIGHT_PADDLE].direction = NO_DIRECTION;
						} else {
							paddles[RIGHT_PADDLE].direction = DIRECTION_DOWN;
						}
						break;
					case XK_q:
						printf("q button down event\n");
						return EXIT_PROGRAM;
				}
				break;
			case KeyRelease: /* Go to function from here */
				//printf("KeyRelease event\n");
				if(paddles[RIGHT_PADDLE].user == COMPUTER)
					break;
				keysym = XLookupKeysym(&event_x11.xkey,0);
				switch(keysym) {
					case XK_Up:
						//printf("Up arrow release event\n");
						paddles[RIGHT_PADDLE].up_button_pressed = 0;
						if(paddles[RIGHT_PADDLE].down_button_pressed) 
							paddles[RIGHT_PADDLE].direction = DIRECTION_DOWN;
						else {
							paddles[RIGHT_PADDLE].direction = NO_DIRECTION;
						}
						break;
					case XK_Down:
						//printf("Down arrow release event\n");
						paddles[RIGHT_PADDLE].down_button_pressed = 0;
						if(paddles[RIGHT_PADDLE].up_button_pressed) 
							paddles[RIGHT_PADDLE].direction = DIRECTION_UP;
						else {
							paddles[RIGHT_PADDLE].direction = NO_DIRECTION;
						}
						break;
				}
				break;
			default:
				printf("Unaccounted event with event number: %d\n",event_x11.type);
				break;
		}
		/* usleep(100000);  REMOVE THIS AFTER DONE, FOR DEBUGGING ONLY */
	}
	return SUCCESS;
}

void move_ball(struct ball_struct *ball)
{
	if(ball->slope == 0) {
		ball->x_pos += ball->velocity;
	} else {
		ball->x_pos += ball->velocity / sqrt(1 + pow(ball->slope,2));
		ball->y_pos += (ball->velocity * ball->slope) / sqrt(1 + pow(ball->slope,2));
	}
}

void
update_moving_window_coordinates(struct window_struct *windows, struct ball_struct *ball, struct paddle_struct *paddles)
{
	/* by default, assuming none of both down and up arrows are held down */
	//printf("Moving windows...\n");
	printf("random reaction: %f\n",random_reaction);

	/* TODO, for larger programs, Create array of objects with non-0 velocity and iterate them here and in move */
	paddles[LEFT_PADDLE].old_y_pos = paddles[LEFT_PADDLE].window->window_y_pos;
	printf("%f\n",ball->x_pos);
	if(ball->velocity > 0 || ball->x_pos > 320 + random_reaction) {
		paddles[LEFT_PADDLE].velocity = 0;
		paddles[LEFT_PADDLE].position_changed = 0;
	} else if(ball->y_pos > paddles[LEFT_PADDLE].window->window_y_pos + paddles[LEFT_PADDLE].height - 20) {
		paddles[LEFT_PADDLE].velocity = paddles[LEFT_PADDLE].default_velocity;
		paddles[LEFT_PADDLE].window->window_y_pos += paddles[LEFT_PADDLE].velocity;
		paddles[LEFT_PADDLE].position_changed = 1;
	} else if(ball->y_pos < paddles[LEFT_PADDLE].window->window_y_pos + 20) {
		paddles[LEFT_PADDLE].velocity = -paddles[LEFT_PADDLE].default_velocity;
		paddles[LEFT_PADDLE].window->window_y_pos += paddles[LEFT_PADDLE].velocity;
		paddles[LEFT_PADDLE].position_changed = 1;
	}

	paddles[RIGHT_PADDLE].old_y_pos = paddles[RIGHT_PADDLE].window->window_y_pos;
	if(paddles[RIGHT_PADDLE].direction == NO_DIRECTION) {
		paddles[RIGHT_PADDLE].velocity = 0;
		paddles[RIGHT_PADDLE].position_changed = 0;
	} else {
		paddles[RIGHT_PADDLE].velocity = paddles[RIGHT_PADDLE].default_velocity;
		paddles[RIGHT_PADDLE].window->window_y_pos += paddles[RIGHT_PADDLE].direction * paddles[RIGHT_PADDLE].velocity;
		paddles[RIGHT_PADDLE].position_changed = 1;
		/* TODO move to after collision detection in new function */
	}

	move_ball(ball);
	/* old, working logic trying to improve, new logic decreases memory and instructions on repeat presses
	 * by around 70-80%
	combined = up_arrow_button + down_arrow_button;
	if (combined && combined != 2) { 
		if (up_arrow_button) { 
			user_paddle_direction = UP_DIRECTION;
		} else {              
			user_paddle_direction = DOWN_DIRECTION;
		}
		windows[user_paddle].window_y_pos += user_paddle_direction * user_paddle_velocity; 
		XMoveWindow(display, windows[user_paddle].window_number, windows[user_paddle].window_x_pos, \
			    windows[user_paddle].window_y_pos);
	}
	*/
}

void
window_collision_detection(struct paddle_struct *paddles, struct ball_struct *ball)
	/* Detect collision on windows with a velocity */
{
	/* TODO NO_DIRECTION should set velocity at 0 */
	//printf("Collision detection...\n");
	float paddle_traveled;
	
	/* If we had a velocity, we moved and so detect collision and re-evaluate if our position changed */
	/* left paddle wall collision */
	if(paddles[LEFT_PADDLE].velocity) {
		if(paddles[LEFT_PADDLE].window->window_y_pos < 0) {
			paddles[LEFT_PADDLE].window->window_y_pos = 1; 
			paddles[LEFT_PADDLE].velocity = 0;
			if(paddles[LEFT_PADDLE].window->window_y_pos == paddles[LEFT_PADDLE].old_y_pos)
				paddles[LEFT_PADDLE].position_changed = 0;
		} else if (paddles[LEFT_PADDLE].window->window_y_pos > 389) { /* TODO window-height - paddle-length */
			paddles[LEFT_PADDLE].window->window_y_pos = 389; /* TODO window-height - paddle-length */
			paddles[LEFT_PADDLE].velocity = 0;
			if(paddles[LEFT_PADDLE].window->window_y_pos == paddles[LEFT_PADDLE].old_y_pos)
				paddles[LEFT_PADDLE].position_changed = 0;
		}
	}
	/* right paddle wall collision */
	if(paddles[RIGHT_PADDLE].velocity) {
		if(paddles[RIGHT_PADDLE].window->window_y_pos < 0) {
			paddles[RIGHT_PADDLE].window->window_y_pos = 1; 
			paddles[RIGHT_PADDLE].velocity = 0;
			if(paddles[RIGHT_PADDLE].window->window_y_pos == paddles[RIGHT_PADDLE].old_y_pos)
				paddles[RIGHT_PADDLE].position_changed = 0;
		} else if (paddles[RIGHT_PADDLE].window->window_y_pos > 389) { /* TODO window-height - paddle-length */
			paddles[RIGHT_PADDLE].window->window_y_pos = 389; /* TODO window-height - paddle-length */
			paddles[RIGHT_PADDLE].velocity = 0;
			if(paddles[RIGHT_PADDLE].window->window_y_pos == paddles[RIGHT_PADDLE].old_y_pos)
				paddles[RIGHT_PADDLE].position_changed = 0;
		}
	}



	/* Ball is past left paddle */
	paddle_traveled = paddles[LEFT_PADDLE].window->window_y_pos - paddles[LEFT_PADDLE].old_y_pos;
	if(ball->passed == False && ball->x_pos <= 20 + 15 - 1) { 
		if (ball->y_pos + ball->width - 1 < paddles[LEFT_PADDLE].window->window_y_pos || ball->y_pos > paddles[LEFT_PADDLE].window->window_y_pos + paddles[LEFT_PADDLE].height - 1) { /* Ball passed right paddle, left player wins */
			ball->passed = True;
			paddles[LEFT_PADDLE].score += 1;
			printf("Right player won :) \n");
		} else { /* Ball collided with right paddle, continue */
			printf("Ball collision with left paddle!\n");
			if(paddle_traveled > 0) { /* If paddle moved down */
				ball->slope -= .3;
				if(ball->slope <= 0)
					ball->slope *= -1;
				printf("moved down collision\n");
			} else if(paddle_traveled < 0) { /* If paddle moved up */
				ball->slope += .3;
				if(ball->slope > 0)
					ball->slope *= -1;
				printf("moved up collision\n");
			} else if (ball->slope) {
				ball->slope *= -1;
			}
			
			/* Limit how much ball bounces off walls */
			if(ball->slope > 1.6) {
				printf("Readjusting\n");
				ball->slope -= .3;
			}
			if(ball->slope < -1.6) {
				printf("Readjusting\n");
				ball->slope += .3;
			}
			ball->velocity = ball->velocity * -1;
			move_ball(ball);
		}
	}

	/* Ball is past right paddle */
	paddle_traveled = paddles[RIGHT_PADDLE].window->window_y_pos - paddles[RIGHT_PADDLE].old_y_pos;
	if(ball->passed == False && ball->x_pos + ball->width - 1 >= 600) { 
		if (ball->y_pos + ball->width - 1 < paddles[RIGHT_PADDLE].window->window_y_pos || ball->y_pos > paddles[RIGHT_PADDLE].window->window_y_pos + paddles[RIGHT_PADDLE].height - 1) { /* Ball passed right paddle, left player wins */
			ball->passed = True;
			paddles[LEFT_PADDLE].score += 1;
			printf("Left player won :) \n");
		} else { /* Ball collided with right paddle, continue */
			printf("Ball collision with right paddle!\n");
			random_reaction = rand() % 51;
			if(paddle_traveled > 0) { /* If paddle moved down */
				ball->slope += .42;
				if(ball->slope > 0)
					ball->slope *= -1;
				printf("moved down collision\n");
			} else if(paddle_traveled < 0) { /* If paddle moved up */
				ball->slope -= .42;
				if(ball->slope <= 0)
					ball->slope *= -1;
				printf("moved up collision\n");
			} else if (ball->slope) {
				ball->slope *= -1;
			}
			
			/* Limit how much ball bounces off walls */
			if(ball->slope > 1.6) {
				printf("Readjusting\n");
				ball->slope -= .42;
			}
			if(ball->slope < -1.6) {
				printf("Readjusting\n");
				ball->slope += .42;
			}
			ball->velocity = ball->velocity * -1;
			move_ball(ball);
		}
	}

	/* Ball hits walls */
	if(ball->y_pos < 0) {
		ball->y_pos = 1;
		ball->slope *= -1;
	} else if (ball->y_pos + ball->width - 1 > 480) {
		ball->y_pos = 480 - ball->width;
		ball->slope *= -1;
	}


	printf("slope: %f\n",ball->slope);


	/* TODO add those with position changed to array for later move by z axis */
}

void
move_windows(struct paddle_struct *paddles, struct ball_struct *ball) {

	/* Move windows that changed position */

	if(paddles[LEFT_PADDLE].position_changed) {
		XMoveWindow(display, paddles[LEFT_PADDLE].window->window_number, paddles[LEFT_PADDLE].window->window_x_pos, \
			    paddles[LEFT_PADDLE].window->window_y_pos);
	}

	if(paddles[RIGHT_PADDLE].position_changed) {
		XMoveWindow(display, paddles[RIGHT_PADDLE].window->window_number, paddles[RIGHT_PADDLE].window->window_x_pos, \
			    paddles[RIGHT_PADDLE].window->window_y_pos);
	}

	XMoveWindow(display, ball->window->window_number, ball->x_pos, ball->y_pos); 

}

void
cleanup(struct window_struct *windows, int num_windows)
{

	int i;
	/* Cleanup
	 * Unmap windows, destroy windows and close display */
	for (i = num_windows - 1; i >= 0; i--) {
		XUnmapWindow(display,windows[i].window_number);
		XDestroyWindow(display,windows[i].window_number);
	}
	XCloseDisplay(display);
	printf("Exiting...\n");
	exit(EXIT_SUCCESS);

}

int 
main(void)
{

	/* Initialize variables */
	struct window_struct windows[MAX_WINDOWS];
	int num_windows = 0;
	struct paddle_struct paddles[2];
	struct ball_struct ball;
	unsigned long frame;
	int num_events;
	memset(paddles, 0, sizeof(struct paddle_struct) * 2); /* 0 out both paddles */
	memset(&ball,0,sizeof(struct ball_struct));
	paddles[LEFT_PADDLE].user = COMPUTER;
	/* some lines below are redundant and here for clarity or future modification */
	paddles[LEFT_PADDLE].direction = NO_DIRECTION;
	paddles[RIGHT_PADDLE].user = HUMAN; 
	paddles[RIGHT_PADDLE].direction = NO_DIRECTION;
	paddles[RIGHT_PADDLE].default_velocity = 4;
	paddles[RIGHT_PADDLE].velocity = paddles[RIGHT_PADDLE].default_velocity;
	paddles[LEFT_PADDLE].velocity = 0;
	paddles[LEFT_PADDLE].default_velocity = 4;
	ball.velocity = 4;
	ball.slope = 0;

	/* Init rand */
	srand(time(NULL));

	/* Open display and get screen number */
	display = XOpenDisplay(NULL);
	screen_number = XDefaultScreen(display);

	/* Make X server send repeated KeyPress events if a key is held down instead of 
	 * rapidly alternating between sending KeyPress and KeyRelease events. 
	 * In other words, make it behave as one would expect it behave. */
	XkbSetDetectableAutoRepeat(display,1,0);
	
	/* Create base window and map to display */
	create_and_map_window(windows, XRootWindow(display,screen_number), &num_windows, 
		KeyReleaseMask | KeyPressMask,
		WHITE_WINDOW_BACKGROUND, 0, 0, 640, 480);
	/* Create left paddle window and map to display */
	create_and_map_window(windows, windows[0].window_number, &num_windows, 0,
		BLACK_WINDOW_BACKGROUND, 20, 20, 15, 91);
	/* Assign left paddle window to left paddle struct */
	paddles[LEFT_PADDLE].window = &windows[num_windows - 1];
	paddles[LEFT_PADDLE].height = 90;
	/* Create right paddle window and map to display */
	create_and_map_window(windows, windows[0].window_number, &num_windows, 0,
		BLACK_WINDOW_BACKGROUND, 600, 150, 15, 91);
	/* Assign right paddle window to right paddle struct */
	paddles[RIGHT_PADDLE].window = &windows[num_windows - 1];
	paddles[RIGHT_PADDLE].height = 90;
	/* Create ball and map to display */
	create_and_map_window(windows, windows[0].window_number, &num_windows, 0,
		BLACK_WINDOW_BACKGROUND, 320, 200, 20, 20);
	/* Assign ball window to ball struct */
	ball.window = &windows[num_windows - 1];
	ball.x_pos = windows[num_windows - 1].window_x_pos;
	ball.y_pos = windows[num_windows - 1].window_y_pos;
	ball.width = 20;

	/* We want to recieve the delete window message
	 * during a ClientMessage event while processing the
	 * event queue */
	Atom wmDeleteMessage = XInternAtom(display,"WM_DELETE_WINDOW",False);
	printf("atom: %lu\n",wmDeleteMessage);
	XSetWMProtocols(display, windows[0].window_number, &wmDeleteMessage, 1);
	wmDeleteMessage = XInternAtom(display,"WM_DELETE_WINDOW",False);
	printf("atom: %lu\n",wmDeleteMessage);
	wmDeleteMessage = XInternAtom(display,"WM_DELETE_WINDOW",False);
	printf("atom: %lu\n",wmDeleteMessage);

	/* MAIN LOOP ---------------------------------------------------------------------------------------------------------------------------- */

	for(frame = 0; ;++frame) {
		/* XSync will display everything from last frame (TODO immediately?), including initial windows.  
		 * It will also wait for all events to come in to process before next sleep */
		XSync(display,False);
		/* Start of frame, */
		num_events = XPending(display);
		//printf("\n\nStart of frame %lu. Number of events: %d\n",frame,num_events);
		/* Get current key states from event queue and process everything else */
		if (num_events) {
			if(process_event_queue(num_events, paddles, wmDeleteMessage) == EXIT_PROGRAM)
				break;
			/* Process non-movement key states */
		}
		/* Update window coordinates */
		update_moving_window_coordinates(windows, &ball, paddles);
		/* Collision detection */
		window_collision_detection(paddles, &ball);
		/* Move everything that was updated */
		move_windows(paddles,&ball);
		/* Wait until end of frame to display, in order for display to be at regular intervals */
		usleep(FRAME_DURATION);
		/*printf("Frame: %lu\n",frame);*/
	} 

	/* -------------------------------------------------------------------------------------------------------------------------------------- */

	/* Exit program */
	cleanup(windows, num_windows);
	return 0;

}
