/* SPDX-License-Identifier: BSD-2-Clause */

#include <time.h>
#include <errno.h>

#include "solo5.h"

#define NSEC_PER_SEC 1000000000L

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
  solo5_time_t tval;

  if (!tp) {
    errno = EFAULT;
    return -1;
  }

  switch (clk_id) {
    case CLOCK_REALTIME:
      tval = solo5_clock_wall();
      break;
    case CLOCK_MONOTONIC:
      tval = solo5_clock_monotonic();
      break;
    default:
      errno = EINVAL;
      return -1;
  }

  tp->tv_sec = tval / NSEC_PER_SEC;
  tp->tv_nsec = tval % NSEC_PER_SEC;

  return 0;
}
