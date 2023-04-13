all:a1 a2

a1:
	as -32 test.S -o test.o
	objcopy -O binary test.o test.bin

a2:
	gcc qemu.c -o light-qemu.run

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.run
