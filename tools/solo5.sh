#!/bin/bash

CONFIG="{\\\"gateway\\\":\\\"10.0.0.1\\\",\\\"interfaces\\\":[{\\\"ip\\\":\\\"10.0.0.2\\\",\\\"masklen\\\":\\\"16\\\",\\\"mac\\\":\\\"00:0d:0b:94:4e:97\\\",\\\"name\\\":\\\"tap\\\",\\\"type\\\":\\\"rumpfd\\\"}],\\\"debug\\\":\\\"1\\\",\\\"singlecpu\\\":\\\"1\\\",\\\"delay_main\\\":\\\"50000\\\",\\\"sysctl\\\":\\\"net.ipv4.tcp_wmem=4096\ 87380\ 2147483647\\\"}"
# XXX: solo5 tenders require explicitly specify devices.
EXEC_HEAP_FLAG=""
MEM="512"
ROOTFS="disk.img"
TAP="tap100"
KERNEL=""
ENVIRON=""
ARGS=""

ENVIRON="${ENVIRON} \"__RUMP_FDINFO_NET_tap=4\""

[ "${SOLO5_EXEC_HEAP}" == 1 ] && EXEC_HEAP_FLAG="--x-exec-heap"

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
  ${EXEC_HEAP_FLAG} \
  --mem=${MEM} \
  --block:rootfs=${ROOTFS} \
  --net:tap=${TAP} \
  ${KERNEL} \
  ${ENVIRON} \
  --config ${CONFIG} \
  ${ARGS}
