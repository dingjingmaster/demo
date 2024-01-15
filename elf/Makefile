cflag = -w

ARCH ?= amd64

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/elf-*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

elfFlags = -I$(curDir)/elf-loader -pipe -Wall -Wextra -fPIC -fno-ident -fno-stack-protector -U _FORTIFY_SOURCE -DELFCLASS=ELFCLASS64 -g
elfLoaderFlags = -nostartfiles -nodefaultlibs -nostdlib -pie -e z_start -Wl,-Bsymbolic,--no-undefined,--build-id=none

#elf-loader/amd64/z_start.o elf-loader/amd64/z_syscall.o elf-loader/amd64/z_trampo.o #elf-loader/i386/z_start.o elf-loader/i386/z_syscall.o elf-loader/i386/z_trampo.o
elfObjs = $(strip $(patsubst %.S, %.o, $(strip $(subst $(curDir), ., $(wildcard $(curDir)/elf-loader/$(ARCH)/*.S)))))

all:$(target)

elf-loader.run:elf-loader.o $(elfObjs) elf-loader/z_err.o elf-loader/z_utils.o elf-loader/z_printf.o elf-loader/z_syscalls.o
	@gcc -o $@ $^ $(elfLoaderFlags) $(elfFlags)
	@echo "LD $@"

%.run:%.o
	@gcc -o $@ $< `pkg-config --libs glib-2.0 libelf` $(cflag) 
	@echo "LD $@"

%.o:%.c
	@gcc `pkg-config --cflags glib-2.0 libelf` -o $@ -c $< $(cflag)
	@echo "cc $@"

elf-loader.o:elf-loader.c
	@gcc -o $@ -c $< $(elfFlags)
	@echo "cc -o $@"

elf-loader/%.o:elf-loader/%.c
	@gcc -o $@ -c $< $(elfFlags)
	@echo "cc -o $@"

elf-loader/amd64/%.o:elf-loader/amd64/%.S
	@gcc -o $@ -c $< $(elfFlags)
	@echo "cc -o $@"

elf-loader/i386/%.o:elf-loader/i386/%.S
	@gcc -m32 -o $@ -c $< $(elfFlags)
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

