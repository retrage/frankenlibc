#ifndef _JS_RWLOCK_H_
#define _JS_RWLOCK_H_

struct krwlock {
  volatile uintptr_t rw_owner;
};

#endif /* _JS_RWLOCK_H_ */
