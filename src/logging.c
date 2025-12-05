#include "logging.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

/* Initialize globals */
uint8_t logging_mode = LOGGING_MODE_NORMAL;

void
logging_turn_off_buffering(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

}

void
logging_log_stdout(char *str)
{

    if(logging_mode != LOGGING_MODE_SILENT)
        puts(str);

}

void
logging_log_error(char *str)
{

    if(logging_mode != LOGGING_MODE_SILENT)
        fprintf(stderr, "Error: %s\n", str);

}

void
logging_log_sys_error(char *str, int err)
{

    if(logging_mode != LOGGING_MODE_SILENT)
        fprintf(stderr, "Error: %s: %s\n", str, strerror(err));

}
