/* SPDX-License-Identifier: BSD-2-Clause */

#include <sys/uio.h>
#include <errno.h>

#include "solo5.h"

extern solo5_handle_t solo5_net_handle;

ssize_t
readv(int fd, const struct iovec *iov, int iovcnt)
{
  int i;
  ssize_t count;
  size_t read_size;
  solo5_result_t res;

  count = 0;

  if (fd == SOLO5_NET_FD) {
    for (i = 0; i < iovcnt; i++) {
      res = solo5_net_read(solo5_net_handle, (uint8_t *)iov[i].iov_base,
          iov[i].iov_len, &read_size);
      switch (res) {
        case SOLO5_R_OK:
          count += read_size;
          break;
        case SOLO5_R_AGAIN:
          errno = EAGAIN;
          break;
        default:
          errno = EBADF;
          break;
      }
      if (res != SOLO5_R_OK)
        return -1;
    }
  }

  return count;
}
