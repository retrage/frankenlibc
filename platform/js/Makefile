COMPILE.c?=$(CC) -c $(CPPFLAGS) $(CFLAGS)
COMPILE.S?=$(CC) -c $(CPPFLAGS) $(ASFLAGS)
RUMPOBJ?=../../rumpobj
OBJDIR=$(RUMPOBJ)/platform
RUMP?=$(RUMPOBJ)/rump
CRTDIR=$(RUMP)/lib
OBJ=$(CRTDIR)/crt0.o
OBJ+=$(CRTDIR)/crti.o $(CRTDIR)/crtn.o
OBJ+=$(CRTDIR)/crtbegin.o $(CRTDIR)/crtbeginT.o
OBJ+=$(CRTDIR)/crtend.o
OBJ+=$(OBJDIR)/clock_getres.o
OBJ+=$(OBJDIR)/clock_gettime.o
OBJ+=$(OBJDIR)/clock_nanosleep.o
OBJ+=$(OBJDIR)/_exit.o
OBJ+=$(OBJDIR)/fcntl.o
OBJ+=$(OBJDIR)/fstat.o
OBJ+=$(OBJDIR)/fsync.o
OBJ+=$(OBJDIR)/getpagesize.o
OBJ+=$(OBJDIR)/getrandom.o
OBJ+=$(OBJDIR)/kill.o
OBJ+=$(OBJDIR)/lseek.o
OBJ+=$(OBJDIR)/mmap.o
OBJ+=$(OBJDIR)/mprotect.o
OBJ+=$(OBJDIR)/__libc_start_main.o
OBJ+=$(OBJDIR)/poll.o
OBJ+=$(OBJDIR)/pread.o
OBJ+=$(OBJDIR)/preadv.o
OBJ+=$(OBJDIR)/pwrite.o
OBJ+=$(OBJDIR)/pwritev.o
OBJ+=$(OBJDIR)/read.o
OBJ+=$(OBJDIR)/readv.o
OBJ+=$(OBJDIR)/write.o
OBJ+=$(OBJDIR)/writev.o
HEADERS=
INCLUDES=-I../include -I../../franken/include -I../../franken/init -I../../franken/ucontext -I../../librumpuser -I../../franken/thread -I.
CPPFLAGS+=-nostdinc $(INCLUDES)
CFLAGS+=-fno-stack-protector

default:			$(OBJ)

$(CRTDIR)/crt.c:
				@mkdir -p $(CRTDIR)
				touch $@

$(CRTDIR)/crt0.o: $(CRTDIR)/crt.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ -c $<

$(CRTDIR)/crti.o: $(CRTDIR)/crt.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ -c $<

$(CRTDIR)/crtn.o: $(CRTDIR)/crt.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ -c $<

$(CRTDIR)/crtbegin.o: $(CRTDIR)/crt.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ -c $<

$(CRTDIR)/crtbeginT.o: $(CRTDIR)/crt.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ -c $<

$(CRTDIR)/crtend.o: $(CRTDIR)/crt.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ -c $<

$(CRTDIR)/crt1.o:	crt1.c
				@mkdir -p $(CRTDIR)
				$(COMPILE.c) -o $@ crt1.c

$(OBJDIR)/clock_getres.o:	clock_getres.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ clock_getres.c

$(OBJDIR)/clock_gettime.o:	clock_gettime.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ clock_gettime.c

$(OBJDIR)/clock_nanosleep.o:	clock_nanosleep.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ clock_nanosleep.c

$(OBJDIR)/_exit.o:		_exit.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ _exit.c

$(OBJDIR)/fcntl.o:		fcntl.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ fcntl.c

$(OBJDIR)/fstat.o:		fstat.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ fstat.c

$(OBJDIR)/fsync.o:		fsync.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ fsync.c

$(OBJDIR)/getpagesize.o:	getpagesize.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ getpagesize.c

$(OBJDIR)/getrandom.o:		getrandom.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ getrandom.c

$(OBJDIR)/kill.o:		kill.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ kill.c

$(OBJDIR)/__libc_start_main.o:	__libc_start_main.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ __libc_start_main.c

$(OBJDIR)/lseek.o:		lseek.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ lseek.c

$(OBJDIR)/mmap.o:		mmap.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ mmap.c

$(OBJDIR)/mprotect.o:		mprotect.c
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ mprotect.c

$(OBJDIR)/open.o:		open.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ open.c

$(OBJDIR)/poll.o:		poll.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ poll.c

$(OBJDIR)/pread.o:		pread.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ pread.c

$(OBJDIR)/preadv.o:		preadv.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ preadv.c

$(OBJDIR)/pwrite.o:		pwrite.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ pwrite.c

$(OBJDIR)/pwritev.o:		pwritev.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ pwritev.c

$(OBJDIR)/read.o:		read.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ read.c

$(OBJDIR)/readv.o:		readv.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ readv.c

$(OBJDIR)/write.o:		write.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ write.c

$(OBJDIR)/writev.o:		writev.c $(HEADERS)
				@mkdir -p $(OBJDIR)
				$(COMPILE.c) -o $@ writev.c

.PHONY:				clean
clean:		
				rm -f $(OBJDIR)/*.o $(CRTDIR)/*.o
