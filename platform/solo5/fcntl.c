/* SPDX-License-Identifier: BSD-2-Clause */

#include <fcntl.h>
#include <errno.h>

int fcntl(int fd, int cmd, ...)
{
  /* solo5 block device can be read/write */
  if (fd == SOLO5_ROOTFS_FD && cmd == F_GETFL)
    return O_RDWR;

  if (fd == SOLO5_NET_FD && cmd == F_GETFL)
    return O_RDWR;

  errno = EINVAL;
  return -1;
}
