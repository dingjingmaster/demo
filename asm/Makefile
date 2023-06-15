src = hello-world.asm sample-x86.asm sample-x86_64.asm
obj = $(patsubst %.asm, %.o, $(src))
run = $(patsubst %.o, %.run, $(obj))

src1 = $(wildcard common-*.asm)
obj1 = $(patsubst %.asm, %.o, $(src1))
run1 = $(patsubst common-%.asm, common-%.run, $(src1))

src2 = $(wildcard l-asm*.asm)
obj2 = $(patsubst %.asm, %.o, $(src2))
run2 = $(patsubst %.o, %.run, $(obj2))

all : $(run2) $(run) $(run1)


common-%.run : common-%.o common.o
	ld -o $@ $^ -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc

l-asm-%.run : l-asm-%.asm
	nasm -f bin -o $@ $<
	@echo -e "\e[35m dd if=./$@ of=image.img bs=512 count=1 conv=notrunc && /.run.sh\e[0m"

%.run : %.o
	ld -o $@ $< 

%.o : %.asm
	as -g -o $@ $<

%.o1 : %.s
	as -g -o $@ $<

common.o : common.c
	gcc -g -o $@ -c $<

.PHONY : clean all

clean :
	@rm -f *.o
	rm -f $(run) $(run1)
