/* SPDX-License-Identifier: BSD-2-Clause */

#include <unistd.h>

#include "solo5.h"

void _exit(int status)
{
	solo5_exit(status);
    while (1) {};
}
