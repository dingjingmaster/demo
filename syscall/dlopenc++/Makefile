my_libc_path=$(shell pwd)

all:libs main hook test 


libs:
	g++ -fPIC --shared -Wall -o libtest.so test.cpp -ldl

main:
	g++ main.cpp -fpie -Wl,-rpath=$(my_libc_path) -L$(my_libc_path) -ltest -o main.run 

hook:
	gcc -fpic -shared -Wall -o libhook.so hook.c -ldl

test:
	LD_PRELOAD=$(my_libc_path)/libhook.so && export LD_PRELOAD && echo $(LD_PRELOAD) && ./main.run


.PHONY: clean libs all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.run"`
	@rm -f `find -name "*.so"`
