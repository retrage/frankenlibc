default:	
		./build.sh

seccomp:
		./build.sh seccomp

qemu-arm:
		./build.sh qemu-arm

spike:
		./build.sh spike

PHONY:		clean distclean qemu-arm spike seccomp

clean:		
		rm -rf rumpobj
		$(MAKE) clean -C musl

distclean:	clean
		rm -rf rump
