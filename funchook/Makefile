.PHONY:all funchook


all:funchook main1.run


main1.run:main1.c funchook
	gcc $< -L funchook/build -Wl,-rpath=$(shell pwd)/funchook/build -lpthread $(shell pwd)/funchook/build/libfunchook.a -o $@


funchook:
	cmake -B funchook/build -S funchook
	make -C funchook/build

