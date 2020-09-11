#!/bin/sh

rumpkernel_buildrump()
{
./buildrump/buildrump.sh \
	-V RUMP_CURLWP=hypercall -V RUMP_LOCKS_UP=yes \
	-V MKPIC=no -V RUMP_KERNEL_IS_LIBC=1 \
	-F CPPFLAGS=-Wno-implicit-fallthrough \
	-F CPPFLAGS=-Wno-constant-conversion \
	-F CPPFLAGS=-Wno-void-pointer-to-int-cast \
	-F CPPFLAGS=-Wno-address-of-packed-member \
	-F CPPFLAGS=-Wno-varargs \
	-F CFLAGS=-fno-stack-protector \
	-k -s ${RUMPSRC} -o ${RUMPOBJ} -d ${RUMP} \
	${BUILD_QUIET} ${STDJ} \
	-F CPPFLAGS="${EXTRA_CPPFLAGS}" \
	-F CFLAGS="${EXTRA_CFLAGS}" \
	-F AFLAGS="${EXTRA_AFLAGS}" \
	-F LDFLAGS="${EXTRA_LDFLAGS}" \
	-F CWARNFLAGS="${EXTRA_CWARNFLAGS}" \
	-F DBG="${F_DBG}" \
	${EXTRAFLAGS} \
	tools build kernelheaders install
}

rumpkernel_createuserlib()
{
	usermtree ${RUMP}
	userincludes ${RUMPSRC} ${NETBSDLIBS}

	for lib in ${NETBSDLIBS}; do
		makeuserlib ${lib}
	done
}

rumpkernel_install_header()
{
	# install headers
	cp -a ${RUMP}/include/* ${OUTDIR}/include
}

rumpkernel_install_extra_libs()
{
(
	cd ${RUMP}/lib
	for f in ${USER_LIBS}
	do
		${INSTALL-install} ${RUMP}/lib/lib${f}.a ${OUTDIR}/lib
	done
)

UNDEF="-D__NetBSD__ -D__RUMPRUN__"
[ ${OS} = "linux" ] && appendvar UNDEF "-Ulinux -U__linux -U__linux__ -U__gnu_linux__"
[ ${OS} = "freebsd" ] && appendvar UNDEF "-U__FreeBSD__"
appendvar UNDEF "-U_BIG_ENDIAN -U_LITTLE_ENDIAN"

}

rumpkernel_explode_libc()
{
	cd ${RUMPOBJ}/explode/libc
	${AR-ar} x ${RUMP}/lib/libc.a
	LIBC_DIR=libc
    for OBJ in *.o; do
        opt -load /home/akira/src/llvm/build/lib/LLVMRedefineSyms.so -emns_redefine_syms $OBJ -o $OBJ
    done
}

rumpkernel_build_extra()
{
(
	CFLAGS="${EXTRA_CFLAGS} ${DBG_F} ${FRANKEN_CFLAGS}" \
		LDFLAGS="${EXTRA_LDFLAGS}" \
		CPPFLAGS="${EXTRA_CPPFLAGS} ${RUMPUSER_FLAGS}" \
		RUMPOBJ="${RUMPOBJ}" \
		RUMP="${RUMP}" \
		${MAKE} ${STDJ} -C libvirtif

	cd libtc
	CFLAGS="${EXTRA_CFLAGS} ${DBG_F}" ${RUMPMAKE}
	cp libfranken_tc.a ${RUMP}/lib/
	${RUMPMAKE} clean
)
}

rumpkernel_maketools()
{
	CC="${BINDIR}/${COMPILER}" \
	RUMPSRC=${RUMPSRC} \
	RUMPOBJ=${RUMPOBJ} \
	OUTDIR=${OUTDIR} \
	BINDIR=${BINDIR} \
		${MAKE} ${STDJ} -C utilities
}

rumpkernel_build_test()
{
	return 0
}

rumpkernel_install_libcxx()
{
	echo "Nothing to be done"
}
