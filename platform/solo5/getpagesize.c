/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>

#if defined(__x86_64__)
#define PAGE_SHIFT 12
#else
#error "Unknown architecture"
#endif

#define PAGE_SIZE (1UL << PAGE_SHIFT)

int getpagesize(void)
{
	return PAGE_SIZE;
}
