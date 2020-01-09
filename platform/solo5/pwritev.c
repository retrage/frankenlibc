#include <sys/uio.h>
#include <errno.h>

#include "solo5.h"

#define SOLO5_ROOTFS_FD 3

extern solo5_handle_t solo5_rootfs_handle;
extern struct solo5_block_info *solo5_rootfs_info;

ssize_t
pwritev(int fd, const struct iovec *iov, int iovcnt, off_t off)
{
  int i, j;
  ssize_t count;
  size_t block_size;
  solo5_result_t res;

  if (fd != SOLO5_ROOTFS_FD || !solo5_rootfs_handle)
    return EINVAL;

  block_size = solo5_rootfs_info->block_size;
  if (off & (block_size - 1))
    return EINVAL;

  count = 0;
  for (i = 0; i < iovcnt; i++) {
    if (iov[i].iov_len & (block_size - 1))
      return EINVAL;
    for (j = 0; j * block_size < iov[i].iov_len; j++) {
      res = solo5_block_write(solo5_rootfs_handle,
          (solo5_off_t)(off + count + j * block_size),
          (const uint8_t *)(iov[i].iov_base + j * block_size), block_size);
          if (res != SOLO5_R_OK)
            return EIO;
    }
    count += iov[i].iov_len;
  }

  return count;
}
