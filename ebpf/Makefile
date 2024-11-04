.PHONY:all clean
curDir 			= $(shell pwd)

netMarkBpfSrc 	= ./net-mark.bpf.c
netMarkSrc 		= ./net-mark.c

all:net-mark.run net-mark.bpf.o

net-mark.run:$(netMarkSrc)
	gcc -o $@ $^ -lbpf

net-mark.bpf.o:$(netMarkBpfSrc)
	clang -O2 -target bpf -g -o $@ -c $< 

clean:
	rm -f *.o
	rm -f *.run
