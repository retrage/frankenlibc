/* SPDX-License-Identifier: BSD-2-Clause */

#include <time.h>
#include <errno.h>
#include <poll.h>

#include "init.h"
#include "thread.h"

#include "solo5.h"

#define NSEC_PER_SEC 1000000000ULL

extern struct pollfd __platform_pollfd[1];

extern solo5_handle_t solo5_net_handle;

int clock_nanosleep(clockid_t clk_id, int flags, const struct timespec *request, struct timespec *remain)
{
  solo5_time_t deadline;
  solo5_handle_set_t ready_set;
  struct thread *thread;

  /* we could support these but not needed */
  if (flags != 0 || remain != NULL) {
    errno = EINVAL;
    return -1;
  }

  if (!request) {
    errno = EFAULT;
    return -1;
  }

  ready_set = 0;
  deadline = solo5_clock_monotonic();
  deadline += request->tv_sec * NSEC_PER_SEC + request->tv_nsec;

  solo5_yield(deadline, &ready_set);

  if (ready_set & 1U << solo5_net_handle) {
    __platform_pollfd[0].revents = POLLIN;
    thread = __franken_fd[__platform_pollfd[0].fd].wake;
    if (thread)
      wake(thread);
  }

  return 0;
}
