#ifndef _JS_VMPARAM_H_
#define _JS_VMPARAM_H_

#include <js/param.h>

#define PAGE_SHIFT  PGSHIFT
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#define PAGE_MASK   (PAGE_SIZE - 1)

#ifndef MAXTSIZ
#define MAXTSIZ     (128*1024*1024)     /* max text size */
#endif
#ifndef DFLDSIZ
#define DFLDSIZ     (256*1024*1024)     /* initial data size limit */
#endif
#ifndef MAXDSIZ
#define MAXDSIZ     (1536*1024*1024)    /* max data size */
#endif
#ifndef MAXSSIZ
#define MAXSSIZ     (120*1024*1024)     /* max stack size */
#endif

#ifndef VM_NFREELIST
#define VM_NFREELIST          2
#define VM_FREELIST_DEFAULT   0
#define VM_FREELIST_DIRECTMAP 1
#endif

#ifndef VM_PHYSSEG_MAX
#define VM_PHYSSEG_MAX        1
#endif

#define VM_MAXUSER_ADDRESS  ((vaddr_t)-0x7fffffff-1)/* 0xFFFFFFFF80000000 */

#define VM_MIN_KERNEL_ADDRESS   ((vaddr_t)-0x7fffffff-1)/* 0xFFFFFFFF80000000 */
#define VM_MAX_KERNEL_ADDRESS   ((vaddr_t)-0x40000000)  /* 0xFFFFFFFFC0000000 */

#define VM_MIN_ADDRESS      ((vaddr_t)0x00000000)
#define VM_MAX_ADDRESS      VM_MAXUSER_ADDRESS

#endif /* ! _JS_VMPARAM_H_ */
