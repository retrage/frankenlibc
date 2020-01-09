#include <fcntl.h>
#include <errno.h>

int fcntl(int fd, int cmd, ...)
{
  if (fd != SOLO5_ROOTFS_FD || cmd != F_GETFL)
    return EINVAL;

  /* solo5 block device can be read/write */
  return O_RDWR;
}
