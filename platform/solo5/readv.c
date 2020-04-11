/* SPDX-License-Identifier: BSD-2-Clause */

#include <sys/uio.h>

ssize_t
readv(int fd, const struct iovec *iov, int iovcnt)
{
  return preadv(fd, iov, iovcnt, -1);
}
