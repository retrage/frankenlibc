/* SPDX-License-Identifier: BSD-2-Clause */

#include <poll.h>
#include <stdlib.h>

#include "solo5.h"

extern void solo5_init_mm(const struct solo5_start_info *);
extern void parse_cmdline(const char *);
extern int main(int, char **, char **);
int __franken_start_main(int (*)(int, char **, char **), int, char **, char **);
int argc = 0;
char **argv = NULL;
char **envp = NULL;

int __platform_npoll = 0;
struct pollfd __platform_pollfd[1];

#define SOLO5_ROOTFS_NAME "rootfs"

solo5_handle_t solo5_rootfs_handle = 0;
struct solo5_block_info *solo5_rootfs_info = NULL;
off_t solo5_rootfs_offset = 0;

#define SOLO5_NET_NAME "tap"

solo5_handle_t solo5_net_handle = 0;
struct solo5_net_info *solo5_net_info = NULL;

int solo5_app_main(const struct solo5_start_info *info)
{
	solo5_init_mm(info);

    parse_cmdline(info->cmdline);

    solo5_rootfs_info = malloc(sizeof(struct solo5_block_info));

    solo5_block_acquire(SOLO5_ROOTFS_NAME, &solo5_rootfs_handle,
        solo5_rootfs_info);

    solo5_net_info = malloc(sizeof(struct solo5_net_info));

    solo5_net_acquire(SOLO5_NET_NAME, &solo5_net_handle,
        solo5_net_info);

    __platform_pollfd[__platform_npoll].fd = SOLO5_NET_FD;
    __platform_pollfd[__platform_npoll].events = POLLIN | POLLPRI | POLLOUT;
    __platform_npoll++;

	return __franken_start_main(main, argc, argv, envp);
}
