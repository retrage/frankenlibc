/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>
#include <sys/uio.h>

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
  const struct iovec iov[1] = {
    { .iov_base = buf, .iov_len = count },
  };

  return preadv(fd, iov, 1, offset);
}
