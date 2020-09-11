#ifndef _JS_PARAM_H_
#define _JS_PARAM_H_

#define _MACHINE_ARCH js
#define MACHINE_ARCH  "js"
#define _MACHINE      js
#define MACHINE       "js"

#define DEV_BSHIFT  9
#define DEV_BSIZE   (1 << DEV_BSHIFT)
#define BLKDEV_IOSIZE 2048

#ifndef MAXPHYS
#define MAXPHYS     65536
#endif

#define PGSHIFT     12
#define NBPG        (1 << PGSHIFT)
#define PGOFSET     (NBPG - 1)

#define UPAGES      2
#define USPACE      (UPAGES << PGSHIFT)
#define USPACE_ALIGN  NBPG

#define MSIZE       512

#ifndef MCLSHIFT
#define MCLSHIFT    11
#endif /* MCLSHIFT */

#define MCLBYTES    (1 << MCLSHIFT)

#endif /* ! _JS_PARAM_H_  */
