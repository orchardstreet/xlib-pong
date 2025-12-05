#include "logging.h"
#include "x11_window.h"
#include "x11_display.h"

int main(void)
{

    logging_turn_off_buffering();
    x11_display_init();


    logging_log_stdout("exiting...");
    return 0;
}
