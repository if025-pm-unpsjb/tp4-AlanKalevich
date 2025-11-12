#include "mbed.h"

extern "C" pid_t _getpid();
extern "C" void _kill();

pid_t _getpid(void)
{
    return(-1);
}

void _kill(void)
{
	;
}
