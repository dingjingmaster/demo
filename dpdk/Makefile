curdir          	= $(shell realpath -- $(shell pwd))
SUBDIRS         	= recv #hello-world basicfwd bbdev

export RTE_TARGET   = x86_64-native-linux-gcc
export RTE_SDK      = $(curdir)/dpdk-stable-19.08.2

all:
	@for n in $(SUBDIRS); do $(MAKE) -C $$n || exit 1; done

clean:
	@for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done

