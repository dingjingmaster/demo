cur = $(shell pwd)

all:libtest.so main1.run

libtest.so:libtest.c
	gcc -shared -fPIC $^ -o $@ 

main1.run:main1.c
	gcc -L . -ltest -Wl,-rpath=$(cur) $^ -o $@

clean:
	@rm -f *.so

.PHONY:clean libtest.so main1.run
