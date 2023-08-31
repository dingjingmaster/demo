# Makefile for the simple example kernel.
AS86	=as86 -0 -a
LD86	=ld86 -0
AS	=as --32
LD	=ld
LDFLAGS =-m elf_i386 -Ttext 0 -e startup_32 -s -x -M

all:	Image


run:
	bochs -f bochsrc.bxrc

# objcopy -O 生成原始二进制文件
Image: boot system
	dd bs=32 if=boot of=Image skip=1
	objcopy -O binary -R -S -g -x  system head
	cat head >> Image
	truncate --size 5384 Image

disk: Image
	dd bs=8192 if=Image of=/dev/fd0
	sync;sync;sync

head.o: head.s

system:	head.o 
	$(LD) $(LDFLAGS) head.o -o system > System.map

boot:	boot.s
	$(AS86) -o boot.o boot.s
	$(LD86) -s -o boot boot.o

clean:
	rm -f Image System.map core boot head *.o system
