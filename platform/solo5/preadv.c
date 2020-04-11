/* SPDX-License-Identifier: BSD-2-Clause */

#include <sys/uio.h>
#include <errno.h>

#include "solo5.h"

extern solo5_handle_t solo5_net_handle;
extern struct solo5_net_info *solo5_net_info;

extern solo5_handle_t solo5_rootfs_handle;
extern struct solo5_block_info *solo5_rootfs_info;
extern off_t solo5_rootfs_offset;

static int
solo5_get_errno(solo5_result_t result)
{
  switch (result) {
    case SOLO5_R_OK:
      return 0;
    case SOLO5_R_AGAIN:
      return EAGAIN;
    case SOLO5_R_EINVAL:
      return EINVAL;
    default:
      return -1;
  }
}

static ssize_t
solo5_net_preadv(solo5_handle_t handle, struct solo5_net_info *info,
    const struct iovec *iov, int iovcnt, off_t off)
{
  int i;
  ssize_t count;
  size_t read_size;
  solo5_result_t res;

  if (!handle || !info) {
    errno = EINVAL;
    return -1;
  }

  if (!iov || off > 0) {
    errno = EFAULT;
    return -1;
  }

  count = 0;
  for (i = 0; i < iovcnt; i++) {
    if (!iov[i].iov_base) {
      errno = EFAULT;
      return -1;
    }
    if (iov[i].iov_len < info->mtu + SOLO5_NET_HLEN) {
      errno = EINVAL;
      return -1;
    }
    /* FIXME: iov[i] should be completely filled before go next */
    res = solo5_net_read(handle, (uint8_t *)iov[i].iov_base,
        iov[i].iov_len, &read_size);
    if (res != SOLO5_R_OK) {
      errno = solo5_get_errno(res);
      return -1;
    }
    count += read_size;
  }

  return count;
}

static ssize_t
solo5_block_preadv(solo5_handle_t handle, struct solo5_block_info *info,
    const struct iovec *iov, int iovcnt, off_t off)
{
  int i, j;
  off_t *offset;
  ssize_t count;
  size_t block_size;
  solo5_result_t res;

  if (!handle || !info) {
    errno = EINVAL;
    return -1;
  }

  if (!iov) {
    errno = EFAULT;
    return -1;
  }

  block_size = info->block_size;
  if (off & (block_size - 1)) {
    errno = EINVAL;
    return -1;
  }

  /* XXX: cover readv() behavior */
  offset = (off < 0) ? &solo5_rootfs_offset : &off;

  count = 0;
  for (i = 0; i < iovcnt; i++) {
    if (!iov[i].iov_base) {
      errno = EFAULT;
      return -1;
    }
    /* FIXME: This expects iov_len is multiple of block_size */
    if (iov[i].iov_len & (block_size - 1)) {
      errno = EINVAL;
      return -1;
    }
    for (j = 0; j * block_size < iov[i].iov_len; j++) {
      res = solo5_block_read(handle,
          (solo5_off_t)(*offset + count + j * block_size),
          iov[i].iov_base + j * block_size, block_size);
      if (res != SOLO5_R_OK) {
        errno = solo5_get_errno(res);
        return -1;
      }
    }
    count += iov[i].iov_len;
  }

  *offset += count;

  return count;
}

ssize_t
preadv(int fd, const struct iovec *iov, int iovcnt, off_t off)
{
  switch (fd) {
    case SOLO5_NET_FD:
      return solo5_net_preadv(solo5_net_handle, solo5_net_info,
          iov, iovcnt, off);
    case SOLO5_ROOTFS_FD:
      return solo5_block_preadv(solo5_rootfs_handle, solo5_rootfs_info,
          iov, iovcnt, off);
    default:
      errno = EBADF;
      return -1;
  }
}
