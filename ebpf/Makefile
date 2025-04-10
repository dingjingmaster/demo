.PHONY:all clean
curDir 			= $(shell pwd)

netMarkBpfSrc 	= ./net-mark.bpf.c
netMarkSrc 		= ./net-mark.c

all=net-mark.run net-mark.bpf.o \
	#cgroup-filter.run cgroup-filter.bpf.o

all:$(all)

net-mark.run:$(netMarkSrc)
	gcc -o $@ $^ -lbpf

#cgroup-filter.run:cgroup-filter.c
#	gcc -o $@ $^ -lbpf

net-mark.bpf.o:$(netMarkBpfSrc)
	clang -O2 -target bpf -g -o $@ -c $< 

#cgroup-filter.bpf.o:cgroup-filter.bpf.c
#	clang -O2 -target bpf -g -o $@ -c $< 

clean:
	rm -f *.o
	rm -f *.run
	rm -f $(all)
