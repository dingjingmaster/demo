.PHONY:all clean
curDir 			= $(shell pwd)

clangFlags		= \
	-I /usr/src/linux-$(shell uname -r)/include \
	-I /usr/src/linux-$(shell uname -r)/include/uapi \
	-I /usr/src/linux-$(shell uname -r)/arch/x86/include \
	-I /usr/src/linux-$(shell uname -r)/include/generated/uapi \
	-I /usr/src/linux-$(shell uname -r)/arch/x86/include/generated/ 

netMarkBpfSrc 	= ./net-mark.bpf.c
netMarkSrc 		= ./net-mark.c

all = net-mark.run net-mark.bpf.o \
	  file-open.bpf.o

all:$(all)

net-mark.run:$(netMarkSrc)
	gcc -o $@ $^ -lbpf

#cgroup-filter.run:cgroup-filter.c
#	gcc -o $@ $^ -lbpf

file-open.bpf.o:file-open.bpf.c
	clang -g -O2 -target bpf ${clangFlags} -D__TARGET_ARCH_x86 -c $< -o $@

net-mark.bpf.o:$(netMarkBpfSrc)
	clang -O2 -target bpf -g -o $@ -c $< 

#cgroup-filter.bpf.o:cgroup-filter.bpf.c
#	clang -O2 -target bpf -g -o $@ -c $< 

clean:
	rm -f *.o
	rm -f *.run
	rm -f $(all)
