src = $(wildcard *.s)
obj = $(patsubst %.s, %.o, $(src))
run = $(patsubst %.o, %.run, $(obj))


all : $(run) 


# @echo -e "\e[35m dd if=./$@ of=image.img bs=512 count=1 conv=notrunc && /.run.sh\e[0m"
%.run : %.s
	@nasm -f bin -o $@ $<
	@dd if=./$@ of=image.img bs=512 count=1 conv=notrunc >/dev/null 2>&1
	@echo "execute command: bochs -f ./bochsrc.conf"


.PHONY : clean all

clean :
	@rm -f *.o *.run *.img
	@rm -f $(run)
