# SPDX-License-Identifier: BSD-2-Clause
COMPILE.c?=$(CC) -c $(CPPFLAGS) $(CFLAGS)
COMPILE.S?=$(CC) -c $(CPPFLAGS) $(ASFLAGS)
SOLO5DIR?=../../solo5
RUMPOBJ?=../../rumpobj
OBJDIR=$(RUMPOBJ)/platform
RUMP?=$(RUMPOBJ)/rump
CRTDIR=$(RUMP)/lib
CRTFILES=$(CRTDIR)/crt0.o
CRTFILES+=$(CRTDIR)/crti.o $(CRTDIR)/crtn.o
CRTFILES+=$(CRTDIR)/crtbegin.o $(CRTDIR)/crtbeginT.o
CRTFILES+=$(CRTDIR)/manifest.o
CRTFILES+=$(CRTDIR)/crtend.o
OBJ=$(CRTFILES)
OBJ+=$(OBJDIR)/argv.o
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
OBJ+=$(OBJDIR)/pwrite.o
OBJ+=$(OBJDIR)/preadv.o
OBJ+=$(OBJDIR)/pwritev.o
OBJ+=$(OBJDIR)/read.o
OBJ+=$(OBJDIR)/readv.o
OBJ+=$(OBJDIR)/write.o
OBJ+=$(OBJDIR)/writev.o
OBJ+=$(OBJDIR)/lkl_config.o
OBJ+=$(OBJDIR)/solo5_$(SOLO5TARGET).o
HEADERS=solo5.h mft_abi.h
INCLUDES=-I../include -I../../franken/include -I../../franken/init -I../../franken/ucontext -I../../librumpuser -I../../franken/thread -I.
CPPFLAGS+=-nostdinc $(INCLUDES)
CFLAGS+=-fno-stack-protector
ELFTOOL?=$(SOLO5DIR)/elftool/solo5-elftool

default:			$(OBJ)

$(OBJDIR):			
				mkdir -p $(OBJDIR)

$(CRTDIR):			
				mkdir -p $(CRTDIR)

$(CRTDIR)/crt0.o:		$(CRTDIR)
				printf "\n" | $(COMPILE.c) -o $@ -x c -

$(CRTDIR)/crti.o:		$(CRTDIR)
				printf "\n" | $(COMPILE.c) -o $@ -x c -

$(CRTDIR)/crtn.o:		$(CRTDIR)
				printf "\n" | $(COMPILE.c) -o $@ -x c -

$(CRTDIR)/crtbegin.o:		$(CRTDIR)
				printf "\n" | $(COMPILE.c) -o $@ -x c -

$(CRTDIR)/crtbeginT.o:		$(CRTDIR)
				printf "\n" | $(COMPILE.c) -o $@ -x c -

$(CRTDIR)/crtend.o:		$(CRTDIR)
				printf "\n" | $(COMPILE.c) -o $@ -x c -

$(OBJDIR)/argv.o:	argv.c $(HEADERS)
				$(COMPILE.c) -o $@ argv.c

$(OBJDIR)/clock_getres.o:	clock_getres.c $(HEADERS)
				$(COMPILE.c) -o $@ clock_getres.c

$(OBJDIR)/clock_gettime.o:	clock_gettime.c $(HEADERS)
				$(COMPILE.c) -o $@ clock_gettime.c

$(OBJDIR)/clock_nanosleep.o:	clock_nanosleep.c $(HEADERS)
				$(COMPILE.c) -o $@ clock_nanosleep.c

$(OBJDIR)/_exit.o:		_exit.c $(HEADERS)
				$(COMPILE.c) -o $@ _exit.c

$(OBJDIR)/fcntl.o:		fcntl.c $(HEADERS)
				$(COMPILE.c) -o $@ fcntl.c

$(OBJDIR)/fstat.o:		fstat.c $(HEADERS)
				$(COMPILE.c) -o $@ fstat.c

$(OBJDIR)/fsync.o:		fsync.c $(HEADERS)
				$(COMPILE.c) -o $@ fsync.c

$(OBJDIR)/getpagesize.o:	getpagesize.c $(HEADERS)
				$(COMPILE.c) -o $@ getpagesize.c

$(OBJDIR)/getrandom.o:		getrandom.c $(HEADERS)
				$(COMPILE.c) -o $@ getrandom.c

$(OBJDIR)/kill.o:		kill.c $(HEADERS)
				$(COMPILE.c) -o $@ kill.c

$(OBJDIR)/__libc_start_main.o:	__libc_start_main.c $(HEADERS)
				$(COMPILE.c) -o $@ __libc_start_main.c

$(OBJDIR)/lseek.o:		lseek.c $(HEADERS)
				$(COMPILE.c) -o $@ lseek.c

$(OBJDIR)/mmap.o:		mmap.c $(HEADERS)
				$(COMPILE.c) -o $@ mmap.c

$(OBJDIR)/mprotect.o:		mprotect.c
				$(COMPILE.c) -o $@ mprotect.c

$(OBJDIR)/poll.o:		poll.c $(HEADERS)
				$(COMPILE.c) -o $@ poll.c

$(OBJDIR)/pread.o:		pread.c $(HEADERS)
				$(COMPILE.c) -o $@ pread.c

$(OBJDIR)/preadv.o:		preadv.c $(HEADERS)
				$(COMPILE.c) -o $@ preadv.c

$(OBJDIR)/pwrite.o:		pwrite.c $(HEADERS)
				$(COMPILE.c) -o $@ pwrite.c

$(OBJDIR)/pwritev.o:		pwritev.c $(HEADERS)
				$(COMPILE.c) -o $@ pwritev.c

$(OBJDIR)/read.o:		read.c $(HEADERS)
				$(COMPILE.c) -o $@ read.c

$(OBJDIR)/readv.o:		readv.c $(HEADERS)
				$(COMPILE.c) -o $@ readv.c

$(OBJDIR)/write.o:		write.c $(HEADERS)
				$(COMPILE.c) -o $@ write.c

$(OBJDIR)/writev.o:		writev.c $(HEADERS)
				$(COMPILE.c) -o $@ writev.c

$(CRTDIR)/manifest.o:	$(CRTDIR)/manifest.c
				$(COMPILE.c) -o $@ $(CRTDIR)/manifest.c

$(CRTDIR)/manifest.c:	manifest.json $(HEADERS) $(ELFTOOL)
				$(ELFTOOL) gen-manifest $< $@

$(OBJDIR)/solo5_$(SOLO5TARGET).o: $(SOLO5DIR)/bindings/$(SOLO5TARGET)/solo5_$(SOLO5TARGET).o
				cp -f $< $@

$(OBJDIR)/lkl_config.o:	$(OBJDIR)/lkl_config.c
				$(COMPILE.c) -o $@ $(OBJDIR)/lkl_config.c

$(OBJDIR)/lkl_config.c:	gen-config.sh ../../lkl.json
				bash gen-config.sh ../../lkl.json $@

.PHONY:				clean
clean:		
				rm -f $(OBJ)
