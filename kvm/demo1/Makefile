all:a1 a2
	@echo 'ubuntu 16 可以正常执行, arch 2023会报错，留待查看原因'

a1:
	as -32 test.S -o test.o
	objcopy -O binary test.o test.bin

a2:
	gcc qemu.c -o light-qemu.run

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.run
