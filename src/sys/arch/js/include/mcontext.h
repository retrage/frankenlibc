#ifndef _JS_MCONTEXT_H_
#define _JS_MCONTEXT_H_

typedef  struct {
  int __dummy;
} mcontext_t;

#define _UC_SETSTACK  0x00010000
#define _UC_CLRSTACK  0x00020000
#define _UC_TLSBASE   0x00080000

#define _UC_MACHINE_SP(uc)
#define _UC_MACHINE_PC(uc)
#define _UC_MACHINE_INTRV()

#define _UC_MACHINE_SET_PC(uc, pc)

#if defined(_RTLD_SOURCE) || defined(_LIBC_SOURCE) || defined(__LIBPTHREAD_SOURCE__)
#include <sys/tls.h>

static __inline void *
__lwp_getprivate_fast(void)
{
  return NULL;
}

static __inline void *
__lwp_gettcb_fast(void)
{
  return NULL;
}

static __inline void
__lwp_settcb(void *__tcb)
{
}
#endif /* _RTLD_SOURCE || _LIBC_SOURCE || __LIBPTHREAD_SOURCE__ */

#endif /* ! _JS_MCONTEXT_H_ */
