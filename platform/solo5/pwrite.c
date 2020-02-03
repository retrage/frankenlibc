/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>
#include <errno.h>

ssize_t
pwrite(int fd, const void *buf, size_t count, off_t offset)
{

	return EINVAL;
}
