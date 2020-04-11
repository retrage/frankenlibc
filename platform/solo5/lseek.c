/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>
#include <errno.h>

#include "solo5.h"

extern struct solo5_block_info *solo5_rootfs_info;
extern off_t solo5_rootfs_offset;

off_t lseek(int fd, off_t offset, int whence)
{
  if (fd != SOLO5_ROOTFS_FD) {
    errno = EBADF;
    return -1;
  }

  if (whence == SEEK_SET) {
    if (offset > (off_t)solo5_rootfs_info->capacity) {
      errno = EINVAL;
      return -1;
    }
    solo5_rootfs_offset = offset;
  } else if (whence == SEEK_CUR) {
    if (solo5_rootfs_offset + offset > (off_t)solo5_rootfs_info->capacity) {
      errno = EINVAL;
      return -1;
    }
    solo5_rootfs_offset += offset;
  } else if (whence == SEEK_END) {
    if (offset != 0) {
      errno = EINVAL;
      return -1;
    }
    solo5_rootfs_offset = (off_t)solo5_rootfs_info->capacity;
  } else {
    errno = EINVAL;
    return -1;
  }

  return solo5_rootfs_offset;
}
