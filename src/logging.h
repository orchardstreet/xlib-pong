#ifndef LOGGING_H
#define LOGGING_H
#include <inttypes.h>

/* Log mode variable */
extern uint8_t logging_mode;
enum logging_enum_log_modes {LOGGING_MODE_SILENT, LOGGING_MODE_NORMAL, LOGGING_MODE_DEBUG, LOG_MODE_DEBUG_VERBOSE};

/* Turn off stdout/stderr buffering */
void logging_turn_off_buffering(void);

/* Log to stdout according to current log mode */
void logging_log_stdout(char *str);
void logging_log_error(char *str);
void logging_log_sys_error(char *str, int err);

#endif
