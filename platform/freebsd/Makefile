COMPILE.c?=$(CC) -c $(CPPFLAGS) $(CFLAGS)
COMPILE.S?=$(CC) -c $(CPPFLAGS) $(ASFLAGS)
RUMPOBJ?=../../rumpobj
OBJDIR=$(RUMPOBJ)/platform
RUMP?=$(RUMPOBJ)/rump
CRTDIR=$(RUMP)/lib
OBJ=$(CRTDIR)/crt1.o
OBJ+=$(CRTDIR)/crti.o $(CRTDIR)/crtn.o
OBJ+=$(CRTDIR)/crtbegin.o $(CRTDIR)/crtbeginT.o
OBJ+=$(CRTDIR)/crtend.o
OBJ+=$(OBJDIR)/clock_getres.o $(OBJDIR)/__clock_getres.o
OBJ+=$(OBJDIR)/clock_gettime.o $(OBJDIR)/__clock_gettime.o
OBJ+=$(OBJDIR)/clock_nanosleep.o
OBJ+=$(OBJDIR)/_exit.o
OBJ+=$(OBJDIR)/fcntl.o
OBJ+=$(OBJDIR)/fstat.o $(OBJDIR)/__fstat.o $(OBJDIR)/fsync.o
OBJ+=$(OBJDIR)/getpagesize.o $(OBJDIR)/getrandom.o
OBJ+=$(OBJDIR)/ioctl.o $(OBJDIR)/kill.o
OBJ+=$(OBJDIR)/lseek.o
OBJ+=$(OBJDIR)/mmap.o $(OBJDIR)/__mmap.o
OBJ+=$(OBJDIR)/munmap.o $(OBJDIR)/mprotect.o $(OBJDIR)/nanosleep.o
OBJ+=$(OBJDIR)/__poll.o $(OBJDIR)/poll.o
OBJ+=$(OBJDIR)/pread.o
OBJ+=$(OBJDIR)/pwrite.o $(OBJDIR)/__pwrite.o
OBJ+=$(OBJDIR)/preadv.o $(OBJDIR)/pwritev.o
OBJ+=$(OBJDIR)/read.o
OBJ+=$(OBJDIR)/readv.o
OBJ+=$(OBJDIR)/_start1.o
OBJ+=$(OBJDIR)/sysctl.o $(OBJDIR)/write.o
OBJ+=$(OBJDIR)/writev.o
HEADERS=syscall.h i386/syscall.h x86_64/syscall.h freebsd.h
INCLUDES=-I../include -I../../franken/include -I../../franken/init -I../../franken/ucontext -I../../librumpuser -I../../franken/thread
CPPFLAGS+=-nostdinc $(INCLUDES)
CFLAGS+=-fno-stack-protector

default:			$(OBJDIR) $(CRTDIR) $(OBJ)

$(OBJDIR):			
				mkdir -p $(OBJDIR)

$(CRTDIR):			
				mkdir -p $(CRTDIR)

$(CRTDIR)/crt1.o:		crt1.S x86_64/crt1.S i386/crt1.S
				$(COMPILE.S) -o $@ crt1.S

CRT=printf "" | ${CC} ${CFLAGS} -v -x c - -static -o /dev/null 2>&1| grep crti.o | tr ' ' '\n'
CRTI!=$(CRT) | grep crti.o
CRTN!=$(CRT) | grep crtn.o
CRTBEGIN!=$(CRT) | grep crtbegin.o || true
CRTBEGINT!=$(CRT) | grep crtbeginT.o || true
CRTEND!=$(CRT) | grep crtend.o

$(CRTDIR)/crti.o:		$(CRTDIR)
				cp $(CRTI) $@
				chmod u+w $@

$(CRTDIR)/crtn.o:		$(CRTDIR)
				cp $(CRTN) $@
				chmod u+w $@

$(CRTDIR)/crtbegin.o:		$(CRTDIR)
				$(CRT) | grep crtbegin.o && cp $(CRTBEGIN) $@ && chmod u+w $@ || true

$(CRTDIR)/crtbeginT.o:		$(CRTDIR)
				$(CRT) | grep crtbeginT.o && cp $(CRTBEGINT) $@ && chmod u+w $@ || true

$(CRTDIR)/crtend.o:		$(CRTDIR)
				cp $(CRTEND) $@
				chmod u+w $@

$(OBJDIR)/__clock_getres.o:	__clock_getres.S $(HEADERS)
				$(COMPILE.S) -o $@ __clock_getres.S

$(OBJDIR)/clock_getres.o:	clock_getres.c $(HEADERS)
				$(COMPILE.c) -o $@ clock_getres.c

$(OBJDIR)/__clock_gettime.o:	__clock_gettime.S $(HEADERS)
				$(COMPILE.S) -o $@ __clock_gettime.S

$(OBJDIR)/clock_gettime.o:	clock_gettime.c $(HEADERS)
				$(COMPILE.c) -o $@ clock_gettime.c

$(OBJDIR)/clock_nanosleep.o:	clock_nanosleep.c $(HEADERS)
				$(COMPILE.c) -o $@ clock_nanosleep.c

$(OBJDIR)/_exit.o:		_exit.S $(HEADERS)
				$(COMPILE.S) -o $@ _exit.S

$(OBJDIR)/fcntl.o:		fcntl.S $(HEADERS)
				$(COMPILE.S) -o $@ fcntl.S

$(OBJDIR)/fstat.o:		fstat.c $(HEADERS)
				$(COMPILE.c) -o $@ fstat.c

$(OBJDIR)/__fstat.o:		__fstat.S $(HEADERS)
				$(COMPILE.S) -o $@ __fstat.S

$(OBJDIR)/fsync.o:		fsync.S $(HEADERS)
				$(COMPILE.S) -o $@ fsync.S

$(OBJDIR)/getpagesize.o:	getpagesize.c $(HEADERS)
				$(COMPILE.c) -o $@ getpagesize.c

$(OBJDIR)/getrandom.o:		getrandom.c $(HEADERS)
				$(COMPILE.c) -o $@ getrandom.c

$(OBJDIR)/ioctl.o:		ioctl.S $(HEADERS)
				$(COMPILE.S) -o $@ ioctl.S

$(OBJDIR)/kill.o:		kill.S $(HEADERS)
				$(COMPILE.S) -o $@ kill.S

$(OBJDIR)/lseek.o:		lseek.c $(HEADERS)
				$(COMPILE.c) -o $@ lseek.c

$(OBJDIR)/mmap.o:		mmap.c $(HEADERS)
				$(COMPILE.c) -o $@ mmap.c

$(OBJDIR)/__mmap.o:		__mmap.S $(HEADERS)
				$(COMPILE.S) -o $@ __mmap.S

$(OBJDIR)/mprotect.o:		mprotect.S $(HEADERS)
				$(COMPILE.S) -o $@ mprotect.S

$(OBJDIR)/munmap.o:		munmap.S $(HEADERS)
				$(COMPILE.S) -o $@ munmap.S

$(OBJDIR)/nanosleep.o:		nanosleep.S $(HEADERS)
				$(COMPILE.S) -o $@ nanosleep.S

$(OBJDIR)/__poll.o:		__poll.S $(HEADERS)
				$(COMPILE.S) -o $@ __poll.S

$(OBJDIR)/poll.o:		poll.c $(HEADERS)
				$(COMPILE.c) -o $@ poll.c

$(OBJDIR)/pread.o:		pread.S $(HEADERS)
				$(COMPILE.S) -o $@ pread.S

$(OBJDIR)/preadv.o:		preadv.S $(HEADERS)
				$(COMPILE.S) -o $@ preadv.S

$(OBJDIR)/pwrite.o:		pwrite.c $(HEADERS)
				$(COMPILE.c) -o $@ pwrite.c

$(OBJDIR)/__pwrite.o:		__pwrite.S $(HEADERS)
				$(COMPILE.S) -o $@ __pwrite.S

$(OBJDIR)/pwritev.o:		pwritev.S $(HEADERS)
				$(COMPILE.S) -o $@ pwritev.S

$(OBJDIR)/read.o:		read.S $(HEADERS)
				$(COMPILE.S) -o $@ read.S

$(OBJDIR)/readv.o:		readv.S $(HEADERS)
				$(COMPILE.S) -o $@ readv.S

$(OBJDIR)/_start1.o:		_start1.c $(HEADERS)
				$(COMPILE.c) -o $@ _start1.c

$(OBJDIR)/sysctl.o:		sysctl.S $(HEADERS)
				$(COMPILE.S) -o $@ sysctl.S

$(OBJDIR)/write.o:		write.S $(HEADERS)
				$(COMPILE.S) -o $@ write.S

$(OBJDIR)/writev.o:		writev.S $(HEADERS)
				$(COMPILE.S) -o $@ writev.S

.PHONY:				clean
clean:		
				rm -f $(OBJDIR)/*.o $(CRTDIR)/*.o
