src = hello-world.asm sample-x86.asm sample-x86_64.asm
obj = $(patsubst %.asm, %.o, $(src))
run = $(patsubst %.o, %.run, $(obj))

src1 = $(wildcard common-*.asm)
obj1 = $(patsubst %.asm, %.o, $(src1))
run1 = $(patsubst common-%.asm, common-%.run, $(src1))

src2 = $(wildcard l-asm*.asm)
obj2 = $(patsubst %.asm, %.o, $(src2))
run2 = $(patsubst %.o, %.run, $(obj2))

src3 = $(wildcard asm-*.c)
run3 = $(patsubst %.c, %.run, $(src3))

all : $(run3) $(run2) $(run) $(run1)


common-%.run : common-%.o common.o
	ld -o $@ $^ -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc

l-asm-%.run : l-asm-%.asm
	@nasm -f bin -o $@ $<
	@echo -e "\e[35m dd if=./$@ of=image.img bs=512 count=1 conv=notrunc && /.run.sh\e[0m"

%.run : %.o
	ld -o $@ $<
	@echo "LD -o $@ $<"

%.o : %.asm
	@as -g -o $@ $<
	@echo "AS -g -o $@ $<"

%.o1 : %.s
	@as -g -o $@ $<
	@echo "AS -g -o $@ $<"

common.o : common.c
	@gcc -g -o $@ -c $<
	@echo "CC -g -o $@ -c $<"

%.run : %.c
	@gcc -g -o $@ $<
	@echo "CC -g -o $@ $<"

.PHONY : clean all

clean :
	@rm -f *.o
	@rm -f $(run) $(run1)
