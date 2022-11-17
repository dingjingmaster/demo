my_libc_path=$(shell pwd)

all:hook 

hook:
	gcc -w hook.c -fPIC -shared -Wall -o hook.so -ldl


.PHONY: clean libs all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.run"`
	@rm -f `find -name "*.so"`
