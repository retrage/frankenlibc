#ifndef _JS_TYPES_H_
#define _JS_TYPES_H_

#include <sys/cdefs.h>
#include <sys/featuretest.h>
#include <js/int_types.h>

#if defined(_NETBSD_SOURCE)
typedef __UINTPTR_TYPE__ paddr_t;
typedef __UINTPTR_TYPE__ psize_t;
typedef __UINTPTR_TYPE__ vaddr_t;
typedef __UINTPTR_TYPE__ vsize_t;
#define PRIxPADDR   PRIxPTR
#define PRIxPSIZE   PRIxPTR
#define PRIuPSIZE   PRIuPTR
#define PRIxVADDR   PRIxPTR
#define PRIxVSIZE   PRIxPTR
#define PRIuVSIZE   PRIuPTR
#endif

typedef __int32_t register_t;
typedef __uint32_t uregister_t;
#define PRIxREGISTER    PRIx32

#if defined(_KERNEL)
typedef struct label_t {
  int dummy;
} label_t;
#endif

typedef volatile unsigned int __cpu_simple_lock_t;

#define __SIMPLELOCK_LOCKED 1
#define __SIMPLELOCK_UNLOCKED 0

#define __HAVE_TLS_VARIANT_I

#endif /* ! _JS_TYPES_H_  */
