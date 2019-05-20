#!/bin/sh

rumpkernel_buildrump()
{

export RUMP_PREFIX=${PWD}/librumpuser/
export KOPT='buildrump=yes'
export LKL_EXT_OPT="rumprun=no ${KOPT}"

./buildrump/buildrump.sh \
	-V RUMP_CURLWP=hypercall -V RUMP_LOCKS_UP=yes \
	-V MKPIC=no -V RUMP_KERNEL_IS_LIBC=1 \
	-F CFLAGS=-fno-stack-protector \
	-k -s ${RUMPSRC} -o ${RUMPOBJ} -d ${RUMP} \
	${BUILD_QUIET} ${STDJ} \
	-F AFLAGS="${EXTRA_AFLAGS}" \
	${EXTRAFLAGS} \
	-l linux tools build install

}

rumpkernel_createuserlib()
{
# build musl libc for Linux
(
	if [ -z "${BUILD_QUIET}" ] ; then set -x ;fi
	set -e
	echo "=== building musl ==="
	cd musl
	LKL_HEADER="${RUMP}/"
	CIRCLE_TEST_REPORTS="${CIRCLE_TEST_REPORTS-./}"
	./configure --with-lkl=${LKL_HEADER} --disable-shared --enable-debug \
		    --disable-optimize --prefix=${RUMPOBJ}/musl CFLAGS="${EXTRA_CFLAGS}"
	# XXX: bug of musl Makefile ?
	${MAKE} obj/src/internal/version.h
	${MAKE} install
	# install libraries
	${INSTALL-install} -d ${OUTDIR}/lib
	${INSTALL-install} ${RUMPOBJ}/musl/lib/libpthread.a \
			   ${RUMPOBJ}/musl/lib/libcrypt.a \
			   ${RUMPOBJ}/musl/lib/librt.a \
			   ${RUMPOBJ}/musl/lib/libm.a \
			   ${RUMPOBJ}/musl/lib/libdl.a \
			   ${RUMPOBJ}/musl/lib/libutil.a \
			   ${RUMPOBJ}/musl/lib/libresolv.a \
			   ${OUTDIR}/lib
)
}

rumpkernel_install_header()
{
	## FIXME: MUSL_LIBC is somehow misleading as franken also uses.
	## LINUX_LIBC?
	appendvar FRANKEN_FLAGS "-DMUSL_LIBC"
	appendvar EXTRA_CPPFLAGS "-DCONFIG_LKL"
	appendvar EXTRA_CFLAGS "-DCONFIG_LKL"

	# install headers
	cp -a ${RUMP}/include/* ${OUTDIR}/include
	cp -a ${RUMPOBJ}/musl/include/* ${OUTDIR}/include

	# only for mach-o
	if [ "${OS}" = "darwin" ]; then
		cd franken/ucontext && ln -sf /usr/include ./ && cd ../..
	fi

}

[ ${OS} = "freebsd" ] && appendvar UNDEF "-U__FreeBSD__"
[ ${OS} = "darwin" ] && appendvar UNDEF "-U__APPLE__"

rumpkernel_install_extra_libs ()
{
	appendvar UNDEF "-D__linux__ -DCONFIG_LKL -D__RUMPRUN__ -D__linux"
	[ ${OS} == "darwin" ] && appendvar UNDEF "-ffreestanding"
	if [ "${OS}" = "linux" ]; then
	    sudo setcap cap_net_raw=ep ${BINDIR}/rexec \
		|| echo "setcap failed. ignoring"
	fi
	return 0
}

rumpkernel_explode_libc()
{
(
	cd ${RUMPOBJ}/explode/musl
	${AR-ar} x ${RUMPOBJ}/musl/lib/libc.a
	cp ${RUMPOBJ}/${RUMP_KERNEL}.o ./
	# XXX: ld.gold generates _end BSS symbol at link time
	if [ ${OS} = "linux" ] ; then
		objcopy --redefine-sym _end=rumpns__end ${RUMPOBJ}/linux/tools/lkl/liblkl.a
	fi
)
	LIBC_DIR=musl

}

rumpkernel_build_extra()
{
	CFLAGS="${EXTRA_CFLAGS}" \
		${MAKE} -C rumpkernel/ RUMP_KERNEL=${RUMP_KERNEL}
	cp rumpkernel/${RUMP_KERNEL}.o ${RUMPOBJ}/
	${MAKE} -C rumpkernel/ clean RUMP_KERNEL=${RUMP_KERNEL}
	return 0
}

rumpkernel_maketools()
{
	return 0
}

rumpkernel_build_test()
{
	OBJDIR=${RUMPOBJ}/tests

	# XXX qemu-arm still has an issue on pthread_self() during setuid
	if [ ${OS} != "qemu-arm" ] ;
	then
		${MAKE} -C tests/iputils clean
		CC="${BINDIR}/${COMPILER}" LDFLAGS="-static" ${MAKE} -C tests/iputils ping ping6
		cp tests/iputils/ping tests/iputils/ping6 ${OBJDIR}/
		${MAKE} -C tests/iputils clean
	fi
}

rumpkernel_install_libcxx()
{
# build libunwind for Linux
(
        if [ -z "${BUILD_QUIET}" ] ; then set -x ; fi
        set -e
        echo "#define __WORDSIZE 64" > "${OUTDIR}/include/bits/wordsize.h"
        sed -i -e "$ s/#endif/#define __GLIBC_PREREQ(maj, min) 0\n#endif/g" ${OUTDIR}/include/features.h
        echo "=== building libunwind ==="
        mkdir -p ${RUMPOBJ}/libunwind
        cd ${RUMPOBJ}/libunwind
        LIBUNWIND_FLAGS="-I${OUTDIR}/include -D_LIBUNWIND_IS_BAREMETAL=1"
        cmake \
          -DCMAKE_CROSSCOMPILING=True \
          -DCMAKE_C_COMPILER=${OUTDIR}/bin/x86_64-rumprun-linux-clang \
          -DCMAKE_C_FLAGS="${LIBUNWIND_FLAGS}" \
          -DCMAKE_CXX_COMPILER=${OUTDIR}/bin/x86_64-rumprun-linux-clang++ \
          -DCMAKE_CXX_FLAGS="${LIBUNWIND_FLAGS}" \
          -DCMAKE_INSTALL_PREFIX="${OUTDIR}" \
          -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
          -DLIBUNWIND_ENABLE_SHARED=0 \
          -DLIBUNWIND_ENABLE_STATIC=1 \
          -DLIBUNWIND_ENABLE_CROSS_UNWINDING=1 \
          -DLIBUNWIND_TARGET_TRIPLE=x86_64-rumprun-linux \
          -DLLVM_PATH=../../llvm \
          ../../llvm/libunwind
        ${MAKE} VERBOSE=1
        ${MAKE} install
)
# build libc++abi for Linux
(
	if [ -z "${BUILD_QUIET}" ] ; then set -x ; fi
        set -e
        echo "=== building libc++abi ==="
        mkdir -p ${RUMPOBJ}/libcxxabi
        cd ${RUMPOBJ}/libcxxabi
        LIBCXXABI_FLAGS="-I${OUTDIR}/include"
        cmake \
          -DCMAKE_CROSSCOMPILING=True \
          -DCMAKE_C_COMPILER=${OUTDIR}/bin/x86_64-rumprun-linux-clang \
          -DCMAKE_C_FLAGS="${LIBCXXABI_FLAGS}" \
          -DCMAKE_CXX_COMPILER=${OUTDIR}/bin/x86_64-rumprun-linux-clang++ \
          -DCMAKE_CXX_FLAGS="${LIBCXXABI_FLAGS}" \
          -DCMAKE_INSTALL_PREFIX="${OUTDIR}" \
          -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
          -DCMAKE_SHARED_LINKER_FLAGS="-L${OUTDIR}/lib" \
          -DLIBCXXABI_USE_LLVM_UNWINDER=1 \
          -DLIBCXXABI_LIBUNWIND_PATH=../../llvm/unwind \
          -DLIBCXXABI_LIBCXX_INCLUDES=../../llvm/libcxx/include \
          -DLIBCXXABI_ENABLE_SHARED=0 \
          -DLIBCXXABI_ENABLE_STATIC=1 \
          -DLIBCXXABI_BAREMETAL=1 \
          -DLIBCXXABI_TARGET_TRIPLE=x86_64-rumprun-linux \
          -DLLVM_PATH=../../llvm \
          ../../llvm/libcxxabi
        ${MAKE} VERBOSE=1
        ${MAKE} install
)
# build libc++ for Linux
(
	if [ -z "${BUILD_QUIET}" ] ; then set -x ; fi
        set -e
        cp -f rump/include/generated/uapi/linux/version.h rump/include/linux/version.h
        echo "=== building libc++ ==="
        mkdir -p ${RUMPOBJ}/libcxx
        cd ${RUMPOBJ}/libcxx
        LIBCXX_FLAGS="-I${OUTDIR}/include -D_GNU_SOURCE -DPATH_MAX=4096"
        cmake \
          -DCMAKE_CROSSCOMPILING=True \
          -DCMAKE_C_COMPILER=${OUTDIR}/bin/x86_64-rumprun-linux-clang \
          -DCMAKE_C_FLAGS="${LIBCXX_FLAGS}" \
          -DCMAKE_CXX_COMPILER=${OUTDIR}/bin/x86_64-rumprun-linux-clang++ \
          -DCMAKE_CXX_FLAGS="${LIBCXX_FLAGS}" \
          -DCMAKE_INSTALL_PREFIX="${OUTDIR}" \
          -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
          -DCMAKE_SHARED_LINKER_FLAGS="${OUTDIR}/lib" \
          -DLIBCXX_CXX_ABI=libcxxabi \
          -DLIBCXX_CXX_ABI_LIBRARY_PATH="${OUTDIR}/lib" \
          -DLIBCXX_CXX_ABI_INCLUDE_PATHS=../../llvm/libcxxabi/include \
          -DLIBCXX_ENABLE_SHARED=0 \
          -DLIBCXX_ENABLE_STATIC=1 \
          -DLIBCXX_HAS_MUSL_LIBC=1 \
          -DLIBCXX_HAS_GCC_S_LIB=0 \
          -DLIBCXX_TARGET_TRIPLE=x86_64-rumprun-linux \
          -DLLVM_PATH=../../llvm \
          ../../llvm/libcxx
        ${MAKE} VERBOSE=1
        ${MAKE} install
)
}
