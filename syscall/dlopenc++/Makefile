my_libc_path=$(shell pwd)

all:libs main hook test 


libs:
	g++ -w -pipe -fPIC --shared -Wall -o libtest.so test.cpp 

main:
	g++ -w -pipe main.cpp -Wl,-rpath=$(my_libc_path) -L$(my_libc_path) -ltest -o main.run 

hook:
	gcc -w hook.c -fPIC -shared -Wall -o hook.so -ldl

test:
	@LD_PRELOAD="$(LD_PRELOAD) $(my_libc_path)/hook.so" && export LD_PRELOAD && echo $(LD_PRELOAD) && ./main.run


.PHONY: clean libs all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.run"`
	@rm -f `find -name "*.so"`
