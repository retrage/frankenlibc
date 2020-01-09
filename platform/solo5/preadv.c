#include <sys/uio.h>
#include <errno.h>

#include "solo5.h"

#define SOLO5_ROOTFS_FD 3

extern solo5_handle_t solo5_rootfs_handle;
extern struct solo5_block_info *solo5_rootfs_info;

ssize_t
preadv(int fd, const struct iovec *iov, int iovcnt, off_t off)
{
  int i, j;
  off_t offset;
  ssize_t count;
  solo5_result_t res;

  if (fd != SOLO5_ROOTFS_FD || !solo5_rootfs_handle)
    return EINVAL;

  offset = off;
  count = 0;

  for (i = 0; i < iovcnt; i++) {
    for (j = 0; j * solo5_rootfs_info->block_size < iov[i].iov_len; j++) {
      res = solo5_block_read(solo5_rootfs_handle, 
          (solo5_off_t)(offset + j * solo5_rootfs_info->block_size),
          iov[i].iov_base + j * solo5_rootfs_info->block_size,
          solo5_rootfs_info->block_size);
      if (res != SOLO5_R_OK)
        return EIO;
    }
    offset += iov[i].iov_len;
    count += iov[i].iov_len;
  }

  return count;
}
