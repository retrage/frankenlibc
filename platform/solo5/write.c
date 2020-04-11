/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>
#include <sys/uio.h>

ssize_t
write(int fd, const void *buf, size_t count)
{
  const struct iovec iov[1] = {
    { .iov_base = (void *)buf, .iov_len = count },
  };

  return pwritev(fd, iov, 1, -1);
}
