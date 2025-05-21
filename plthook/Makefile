cur = $(shell pwd)

all:libtest.so libtest1.so main.run main1.run main2.run main3.run main4.run main5.run

libtest1.so:libtest1.c
	gcc -shared -fPIC -o $@ $^ 

libtest.so:libtest.c
	gcc -shared -fPIC -o $@ $^ 

main.run:main.c ./plthook/plthook_elf.c
	gcc -ldl $^ -o $@

main1.run:main1.c
	gcc -L . -ltest -Wl,-rpath=$(cur) $^ -o $@

main2.run:main2.c ./plthook/plthook_elf.c
	gcc -L . -ltest1 -Wl,-rpath=$(cur) -ldl $^ -o $@

clean:
	@rm -f *.so *.run

.PHONY:clean libtest.so main1.run main2.run
