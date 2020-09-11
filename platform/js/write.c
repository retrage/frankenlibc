#include <unistd.h>

#define STDOUT_FILENO (1)
#define __WASI_ERRNO_SUCCESS (0)

typedef __SIZE_TYPE__ __wasi_size_t;
typedef uint32_t __wasi_fd_t;
typedef uint16_t __wasi_errno_t;

typedef struct __wasi_ciovec_t {
  const uint8_t * buf; 
  __wasi_size_t buf_len;
} __wasi_ciovec_t;

extern __wasi_errno_t __wasi_fd_write(
  __wasi_fd_t fd, 
  const __wasi_ciovec_t *iovs,
  size_t iovs_len,
  __wasi_size_t *nwritten
) __attribute__((
  __import_module__("wasi_snapshot_preview1"),
  __import_name__("fd_write"),
  __warn_unused_result__
));

extern int __wasi_syscall_ret(__wasi_errno_t code);

ssize_t
write(int fd, const void *buf, size_t count)
{
	const uint8_t *cbuf = buf;
	int i;

    __wasi_ciovec_t iov = {
      .buf = cbuf,
      .buf_len = count 
    };
    __wasi_errno_t error = __wasi_fd_write(STDOUT_FILENO, &iov, 1, &count);
    if (error  != __WASI_ERRNO_SUCCESS) {
      return __wasi_syscall_ret(error);
    }

	return count;
}
