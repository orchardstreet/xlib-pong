#include "logging.h"
#include "util.h"
#include <stdlib.h>

void
util_error_exit(char *str)
{

    logging_log_error(str);
    exit(EXIT_FAILURE);

}
