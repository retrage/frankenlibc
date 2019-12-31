#include <string.h>
#include <stdint.h>

#include "solo5.h"

extern int main(int, char **, char **);
int __franken_start_main(int (*)(int, char **, char **), int, char **, char **);
int argc = 1;
char *argv[2] = {"solo5", "\0"};
char *envp[2] = {"RUMP_VERBOSE=1", "\0"};

int solo5_app_main(const struct solo5_start_info *info)
{
	/* TODO: parse solo5_start_info.cmdline */
	return __franken_start_main(main, argc, argv, envp);
}
