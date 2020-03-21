/* SPDX-License-Identifier: BSD-2-Clause-NetBSD */

#include <stdlib.h>

extern void *__franken_malloc(size_t);
extern void *__franken_calloc(size_t, size_t);
extern void __franken_free(void *);
extern void *__franken_realloc(void *, size_t);

void *
malloc(size_t size)
{
  return __franken_malloc(size);
}

void *
calloc(size_t num, size_t size)
{
  return __franken_calloc(num, size);
}

void
free(void *ptr)
{
  return __franken_free(ptr);
}

void *
realloc(void  *ptr, size_t size)
{
  return __franken_realloc(ptr, size);
}
