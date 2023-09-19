src = $(wildcard *.s)
obj = $(patsubst %.s, %.o, $(src))
run = $(patsubst %.o, %.run, $(obj))


all : $(run) 


%.run : %.s
	nasm -f bin -o $@ $<
	@dd if=./$@ of=image.img bs=512 count=1 conv=notrunc
	#ld86 -0 -s -o $@ $^
	@echo -e "\e[35m dd if=./$@ of=image.img bs=512 count=1 conv=notrunc && /.run.sh\e[0m"

%.o : %.s
	#as86 -0 -a -o $@ $<


.PHONY : clean all

clean :
	@rm -f *.o *.run *.img
	rm -f $(run)
