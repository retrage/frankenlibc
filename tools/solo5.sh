#!/bin/bash

ROOTFS=""
KERNEL=""
ENVIRON=""
ARGS=""

for arg in $(env); do
  ENVIRON="${ENVIRON} \"${arg}\""
done

ARGS_FLAG=""
for arg in "$@"; do
  case ${arg} in
    rootfs:*)
      ROOTFS="$(echo ${arg} | cut -d ":" -f2)"
      ;;
    --|*)
      [ "${arg}" == "--" ] && ARGS_FLAG="1"
      [ -n "${ARGS_FLAG}" ] && ARGS="${ARGS} \"${arg}\""
      [ -z "${ARGS_FLAG}" ] && KERNEL="${arg}"
      ;;
  esac
done

[ -z "${KERNEL}" ] && printf "solo5 kernel is not specified\n" && exit -1
[ -z "${ROOTFS}" ] && printf "rootfs is not specified\n" && exit -1

@TENDER@ \
  --block:rootfs=${ROOTFS} \
  ${KERNEL} \
  ${ENVIRON} \
  ${ARGS}
