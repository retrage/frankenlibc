#include <sys/uio.h>
#include <errno.h>

#include "solo5.h"

extern solo5_handle_t solo5_net_handle;

ssize_t
writev(int fd, const struct iovec *iov, int iovcnt)
{
  int i;
  ssize_t count;
  solo5_result_t res;

  count = 0;

  if (fd == 1 || fd == 2) {
    for (i = 0; i < iovcnt; i++) {
      solo5_console_write((const char *)iov[i].iov_base, iov[i].iov_len);
      count += iov[i].iov_len;
    }
  } else if (fd == SOLO5_NET_FD) {
    for (i = 0; i < iovcnt; i++) {
      res = solo5_net_write(solo5_net_handle, (const char *)iov[i].iov_base,
          iov[i].iov_len);
      switch (res) {
        case SOLO5_R_OK:
          count += iov[i].iov_len;
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
  } else {
    return EINVAL;
  }

  return count;
}
