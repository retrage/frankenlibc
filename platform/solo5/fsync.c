#include <unistd.h>
#include <errno.h>

#define SOLO5_ROOTFS_FD 3

int fsync(int fd)
{
  if (fd != SOLO5_ROOTFS_FD) {
    errno = EINVAL;
    return -1;
  }

  return 0;
}
