#!/bin/sh
# SPDX-License-Identifier: BSD-2-Clause

(cd ${PWD}/solo5 && ./configure.sh && ${MAKE})

MAXPAGESIZE=$(cat ${PWD}/solo5/Makeconf | grep "CONFIG_GUEST_PAGE_SIZE" | cut -d "=" -f2)

appendvar EXTRA_LDFLAGS "-z max-page-size=${MAXPAGESIZE}"
appendvar EXTRA_CPPFLAGS "-D__SOLO5__"
appendvar EXTRA_CPPFLAGS "-DSOLO5_ROOTFS_FD=3"
appendvar EXTRA_CPPFLAGS "-DSOLO5_NET_FD=4"

LINKSCRIPT="${PWD}/platform/solo5/solo5_${SOLO5TARGET}.lds"
EXTRA_LDSCRIPT="-T ${LINKSCRIPT}"
EXTRA_LDSCRIPT_CC="-Wl,-T,${LINKSCRIPT}"
