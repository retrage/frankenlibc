#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

#include "solo5.h"

uintptr_t solo5_heap_start;
uintptr_t solo5_heap_end;

void *
mmap(void *addr, size_t length, int prot, int nflags, int fd, off_t offset)
{
  unsigned int shift;
  uintptr_t align;
  uintptr_t amask;
  int pagesize =  getpagesize();
  void *mem;

  if (fd != -1 || offset != 0 || addr) {
    errno = EINVAL;
    return MAP_FAILED;
  }

  shift = (nflags & MAP_ALIGNMENT_MASK) >> MAP_ALIGNMENT_SHIFT;
  if (shift == 0) {
    align = pagesize;
  } else {
    align = 1UL << shift;
  }
  amask = align - 1UL;

  if ((solo5_heap_start & amask) != 0)
    solo5_heap_start = (solo5_heap_start & ~amask) + align;

  if (solo5_heap_start + length > solo5_heap_end) {
    errno = ENOMEM;
    return MAP_FAILED;
  }

  mem = (void *)solo5_heap_start;

  solo5_heap_start += length;

  return mem;
}

int
munmap(void *addr, size_t length)
{
  return 0;
}
