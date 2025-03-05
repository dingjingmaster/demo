curDir = $(shell pwd)
sdkDir = $(shell dirname $(curDir))/dpdk-stable-19.08.2/x86_64-native-linux-gcc
CFLAGS = -L $(sdkDir)/lib -ldpdk -I $(sdkDir)/include

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.c
	gcc -o $@ $^ $(CFLAGS) 
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete

