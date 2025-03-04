RTE_SDK=/home/dpdk/code/dpdk-stable-19.08.2/
RTE_TARGET=$(RTE_SDK)/x86_64-native-linux-gcc
INCLUDE_PATH=$(RTE_TARGET)/include
LIBS_PATH=$(RTE_TARGET)/lib
LIBS=-ldpdk -lnuma -lpthread -ldl -lrte_mempool_ring

.PHONY:run clean

run:demo-rcv1.run

demo-rcv1.run:rcv1.c
	@gcc -o $@ $^ -I$(INCLUDE_PATH) $(LIBS) -L$(LIBS_PATH) 
	@echo CC -o $@ $^

clean:
	@find -name "*.run" -delete
	@echo clean done!
