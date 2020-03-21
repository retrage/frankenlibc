/* SPDX-License-Identifier: BSD-2-Clause-NetBSD */

#include <sys/types.h>
#include <signal.h>

extern int __platform_kill(pid_t, int);

int kill(pid_t pid, int sig)
{
  return __platform_kill(pid, sig);
}
