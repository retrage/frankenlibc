#include <sys/stat.h>
#include <errno.h>

#include "solo5.h"

extern struct solo5_block_info *solo5_rootfs_info;

int fstat(int fd, struct stat *st)
{
  if (fd != SOLO5_ROOTFS_FD) {
    errno = EINVAL;
    return -1;
  }

  st->st_mode = S_IFBLK;
  st->st_size = (off_t)solo5_rootfs_info->capacity;

  return 0;
}
