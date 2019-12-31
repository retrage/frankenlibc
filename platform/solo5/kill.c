#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "solo5.h"

int kill(pid_t pid, int sig)
{
	solo5_abort();
    return 0;
}
