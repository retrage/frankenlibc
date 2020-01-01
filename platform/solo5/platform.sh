#!/bin/sh

LINKSCRIPT="${PWD}/platform/solo5/solo5_spt.lds"
EXTRA_LDSCRIPT="-T ${LINKSCRIPT}"
EXTRA_LDSCRIPT_CC="-Wl,-T,${LINKSCRIPT}"
