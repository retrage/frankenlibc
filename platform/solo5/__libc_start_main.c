#include <string.h>
#include <stdint.h>

#include "solo5.h"

extern uintptr_t solo5_heap_start;
extern uintptr_t solo5_heap_end;
extern int main(int, char **, char **);
int __franken_start_main(int (*)(int, char **, char **), int, char **, char **);
int argc = 1;
char *argv[2] = {"solo5", "\0"};

int solo5_app_main(const struct solo5_start_info *info)
{
	char **envp;

	solo5_heap_start = info->heap_start;
    solo5_heap_end = info->heap_start + info->heap_size;

    envp = argv + argc + 1;
	/* TODO: parse solo5_start_info.cmdline */
	return __franken_start_main(main, argc, argv, envp);
}
