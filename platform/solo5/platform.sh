#!/bin/sh

(cd ${PWD}/solo5 && ./configure.sh && ${MAKE} )

MAXPAGESIZE=$(cat ${PWD}/solo5/Makeconf | grep "CONFIG_GUEST_PAGE_SIZE" | cut -d "=" -f2)

appendvar EXTRA_LDFLAGS "-z max-page-size=${MAXPAGESIZE}"

if [ "${SOLO5TARGET}" = "spt" ]; then
	LINKSCRIPT="${PWD}/platform/solo5/solo5_spt.lds"
fi
EXTRA_LDSCRIPT="-T ${LINKSCRIPT}"
EXTRA_LDSCRIPT_CC="-Wl,-T,${LINKSCRIPT}"
