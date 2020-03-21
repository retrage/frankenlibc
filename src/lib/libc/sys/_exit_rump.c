/* SPDX-License-Identifier: BSD-2-Clause-NetBSD */

#include <unistd.h>

extern void __platform__exit(int);

void _exit(int status)
{
  __platform__exit(status);
}

/* TODO: use weak symbol alias */
void _Exit(int status)
{
  __platform__exit(status);
}
