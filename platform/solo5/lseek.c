#include <unistd.h>
#include <errno.h>

#include "solo5.h"

extern struct solo5_block_info *solo5_rootfs_info;

off_t lseek(int fd, off_t offset, int whence)
{
  if (fd != SOLO5_ROOTFS_FD)
    return EINVAL;

  if (offset != 0 ||  whence != SEEK_END)
    return EINVAL;

  return (off_t)solo5_rootfs_info->capacity;
}
