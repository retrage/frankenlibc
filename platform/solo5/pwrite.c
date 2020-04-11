/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>
#include <sys/uio.h>

ssize_t
pwrite(int fd, const void *buf, size_t count, off_t offset)
{
  const struct iovec iov[1] = {
    { .iov_base = (void *)buf, .iov_len = count },
  };

  return pwritev(fd, iov, 1, offset);
}
