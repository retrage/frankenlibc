/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>
#include <sys/uio.h>

ssize_t
read(int fd, void *buf, size_t count)
{
  const struct iovec iov[1] = {
    { .iov_base = buf, .iov_len = count },
  };

  return preadv(fd, iov, 1, -1);
}
