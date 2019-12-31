#include <unistd.h>
#include <errno.h>

#include "solo5.h"

ssize_t
write(int fd, const void *buf, size_t count)
{
  solo5_console_write((const char *)buf, count);

  return count;
}
