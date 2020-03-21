/* SPDX-License-Identifier: BSD-2-Clause-NetBSD */

#include <sys/mman.h>

extern int __platform_munmap(void *, size_t);

int
munmap(void *addr, size_t length)
{
  return __platform_munmap(addr, length);
}
