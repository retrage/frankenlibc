#ifndef _JS_MUTEX_H_
#define _JS_MUTEX_H_

#ifndef __MUTEX_PRIVATE

struct kmutex {
  uintptr_t mtx_pad1;
};

#else /* __MUTEX_PRIVATE */

#include <sys/param.h>

struct kmutex {
  volatile uintptr_t mtx_owner;
};

#endif /* __MUTEX_PRIVATE */

#endif /* _JS_MUTEX_H_ */
