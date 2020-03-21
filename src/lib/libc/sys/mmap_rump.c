/* SPDX-License-Identifier: BSD-2-Clause-NetBSD */

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

extern void *__platform_mmap(void *, size_t, int, int, int, off_t);

void *
mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
  return __platform_mmap(addr, length, prot, flags, fd, offset);
}

void *
_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
  return __platform_mmap(addr, length, prot, flags, fd, offset);
}
