#include <unistd.h>
#include <errno.h>

int fsync(int fd)
{
  if (fd != SOLO5_ROOTFS_FD) {
    errno = EINVAL;
    return -1;
  }

  return 0;
}
