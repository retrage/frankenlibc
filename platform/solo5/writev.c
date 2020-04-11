/* SPDX-License-Identifier: BSD-2-Clause */

#include <sys/uio.h>
#include <errno.h>

ssize_t
writev(int fd, const struct iovec *iov, int iovcnt)
{
  return pwritev(fd, iov, iovcnt, -1);
}
