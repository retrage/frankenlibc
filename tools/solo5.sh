#!/bin/bash

# XXX: solo5 tenders require explicitly specify devices.
ROOTFS="disk.img"
TAP="tap100"
KERNEL=""
ENVIRON=""
ARGS=""

ENVIRON="${ENVIRON} \"__RUMP_FDINFO_NET_tap=4\""

for arg in $(env); do
  ENVIRON="${ENVIRON} \"${arg}\""
done

ARGS_FLAG=""
for arg in "$@"; do
  case ${arg} in
    rootfs:*)
      ROOTFS="$(echo ${arg} | cut -d ":" -f2)"
      ;;
    tap:*)
      TAP="$(echo ${arg} | cut -d ":" -f2)"
      ;;
    --|*)
      [ "${arg}" == "--" ] && ARGS_FLAG="1"
      [ -n "${ARGS_FLAG}" ] && ARGS="${ARGS} \"${arg}\""
      [ -z "${ARGS_FLAG}" ] && KERNEL="${arg}"
      ;;
  esac
done

[ -z "${KERNEL}" ] && printf "solo5 kernel is not specified\n" && exit -1
[ ! -e "${ROOTFS}" ] && printf "rootfs ${ROOTFS} does not exist\n" && exit -1
[ -z "$(ip addr show ${TAP})" ] && "tap ${TAP} does not exist\n" && exit -1

@TENDER@ \
  --block:rootfs=${ROOTFS} \
  --net:tap=${TAP} \
  ${KERNEL} \
  ${ENVIRON} \
  ${ARGS}
