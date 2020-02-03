/* SPDX-License-Identifier: BSD-2-Clause */

#include <poll.h>

#include "thread.h"

#include "solo5.h"

#define MSEC_TO_NSEC (1000000ULL)
#define DEFAULT_TIMEOUT (10000ULL)

extern solo5_handle_t solo5_net_handle;

extern struct pollfd __platform_pollfd[1];

int poll(struct pollfd *fds, nfds_t n, int timeout)
{
  solo5_time_t deadline;
  solo5_handle_set_t ready_set;
  int64_t sec;
  long nsec;
  int i, ret;

  ret = 0;
  deadline = solo5_clock_monotonic();

  if (timeout == 0) {
    solo5_yield(deadline, &ready_set);
    if (ready_set & solo5_net_handle) {
      for (i = 0; i < (int)n; i++) {
        if (fds[i].fd == SOLO5_NET_FD) {
          fds[i].revents = POLLIN;
          ret++;
        }
      }
    }
    return ret;
  }

  if (timeout > 0) {
    sec = timeout / 1000;
    nsec = (timeout % 1000) * (1000*1000UL);
  } else {
    sec = 10;
    nsec = 0;
  }
  clock_sleep(CLOCK_REALTIME, sec, nsec);

  if (__platform_pollfd[0].revents) {
    for (i = 0; i < (int)n; i++) {
      if (fds[i].fd == SOLO5_NET_FD) {
        fds[i].revents = POLLIN;
        ret++;
      }
    }
  }

  return ret;

#if 0
  if (n != 1 || fds[0].fd != SOLO5_NET_FD)
    return n;

  if (timeout < 0)
    timeout = DEFAULT_TIMEOUT;

  deadline = solo5_clock_monotonic();

  solo5_yield(deadline, &ready_set);

  if (ready_set & solo5_net_handle)
    return n;

  return n;
#endif
}
