#!/bin/sh

LINKSCRIPT="${PWD}/solo5/bindings/spt/solo5_spt.lds"
EXTRA_LDSCRIPT="-T ${LINKSCRIPT}"
EXTRA_LDSCRIPT_CC="-Wl,-T,${LINKSCRIPT}"
