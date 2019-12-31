#include <time.h>
#include <errno.h>

#include "solo5.h"

#define NSEC_PER_SEC 1000000000L

int clock_nanosleep(clockid_t clk_id, int flags, const struct timespec *request, struct timespec *remain)
{
  solo5_time_t deadline;

  /* we could support these but not needed */
  if (flags != 0 || remain != NULL) {
    errno = EINVAL;
    return -1;
  }

  if (!request) {
    errno = EFAULT;
    return -1;
  }

  deadline = request->tv_sec * NSEC_PER_SEC + request->tv_nsec;

  /* TODO: add network device support */
  solo5_yield(deadline, NULL);

  return 0;
}
