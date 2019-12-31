#include <sys/uio.h>
#include <errno.h>

#include "solo5.h"

ssize_t
writev(int fd, const struct iovec *iov, int iovcnt)
{
  int i;
  ssize_t count;

  count = 0;

  for (i = 0; i < iovcnt; i++) {
    solo5_console_write((const char *)iov[i].iov_base, iov[i].iov_len);
    count += iov[i].iov_len;
  }

  return count;
}
