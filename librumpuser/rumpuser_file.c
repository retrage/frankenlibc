#include "rumpuser_port.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/uio.h>

#include <rump/rumpuser.h>

#include "rumpuser_int.h"

#include "init.h"

int
rumpuser_getfileinfo(const char *path, uint64_t *sizep, int *ftp)
{
	int fd = atoi(path);
	int ft;

	if (fd < 0 || fd >= MAXFD || __franken_fd[fd].valid == 0)
		return ENOENT;

	if (sizep)
		*sizep = __franken_fd[fd].st.st_size;

	switch (__franken_fd[fd].st.st_mode & S_IFMT) {
	case S_IFDIR:
		ft = RUMPUSER_FT_DIR;
		break;
	case S_IFREG:
		ft = RUMPUSER_FT_REG;
		break;
	case S_IFBLK:
		ft = RUMPUSER_FT_BLK;
		break;
	case S_IFCHR:
		ft = RUMPUSER_FT_OTHER;
		break;
	case S_IFIFO:
		ft = RUMPUSER_FT_OTHER;
		break;
	default:
		ft = RUMPUSER_FT_OTHER;
		break;
	}

	if (ftp)
		*ftp = ft;

	return 0;
}

int
rumpuser_open(const char *path, int ruflags, int *fdp)
{
	int fd = atoi(path);

	if (fd < 0 || fd >= MAXFD || __franken_fd[fd].valid == 0)
		return ENOENT;

	*fdp = fd;

	return 0;
}

int
rumpuser_close(int fd)
{

	return 0;
}

int
rumpuser_iovread(int fd, struct rumpuser_iovec *ruiov, size_t iovlen,
	int64_t roff, size_t *retp)
{
	ssize_t n;

	if (roff == RUMPUSER_IOV_NOSEEK || __franken_fd[fd].seek == 0) {
		n = readv(fd, (struct iovec *)ruiov, iovlen);
		if (n == -1)
			return errno;
		*retp = n;
		return 0;
	}

	n = preadv(fd, (struct iovec *)ruiov, iovlen, roff);
	if (n == -1)
		return errno;
	*retp = n;
	return 0;
}

int
rumpuser_iovwrite(int fd, const struct rumpuser_iovec *ruiov, size_t iovlen,
	int64_t roff, size_t *retp)
{
	ssize_t n;

	if (roff == RUMPUSER_IOV_NOSEEK || __franken_fd[fd].seek == 0) {
		n = writev(fd, (struct iovec *)ruiov, iovlen);
		if (n == -1)
			return errno;
		*retp = n;
		return 0;
	}

	n = pwritev(fd, (struct iovec *)ruiov, iovlen, roff);
	if (n == -1)
		return errno;
	*retp = n;
	return 0;
}

int
rumpuser_syncfd(int fd, int flags, uint64_t start, uint64_t len)
{
	int ret;

	ret = fsync(fd);
	if (ret == -1)
		return errno;

	return 0;
}

void
rumpuser_bio(int fd, int op, void *data, size_t dlen, int64_t doff,
        rump_biodone_fn biodone, void *bioarg)
{
	int err = 0;
	ssize_t n = 0;

	assert(biodone != NULL);

	if (__franken_fd[fd].valid == 0)
		biodone(bioarg, 0, EBADF);

	if (op & RUMPUSER_BIO_READ) {
		n = pread(fd, data, dlen, doff);
		if (n == -1) {
			n = 0;
			err = errno;
		}
	} else {
		n = pwrite(fd, data, dlen, doff);
		if (n == -1) {
			n = 0;
			err = errno;
		}
	}
	if (errno == 0 && op & RUMPUSER_BIO_SYNC) {
		if (fsync(fd) == -1) {
			n = 0;
			err = errno;
		}
	}	

	biodone(bioarg, n, err);
}
