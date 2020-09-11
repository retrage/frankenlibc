#ifndef _JS_DISKLABEL_H_
#define _JS_DISKLABEL_H_

#define LABELSECTOR   1
#define LABELOFFSET   0
#define MAXPARTITIONS 16
#define RAW_PART      2

struct cpu_disklabel {
  int __dummy;
};

#endif /* _JS_DISKLABEL_H_ */
