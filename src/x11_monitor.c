#include "x11_monitor.h"
#include <stdlib.h>

x11_monitor *
x11_monitor_new(int monitor_number)
{
    x11_monitor *monitor_struct_ptr = calloc(1, sizeof(x11_monitor));
    monitor_struct_ptr->monitor_number = monitor_number;
    return monitor_struct_ptr;
}
