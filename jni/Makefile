java_home = $(shell find /usr/lib/ -name "java-*-openjdk")

cur_dir = $(PWD)

cflags = -I$(java_home)/include 			\
		 -I$(java_home)/include/linux

.PHONY: all clean


all:test1


test1:
	gcc -o $(cur_dir)/libtest.so $(cflags) -fPIC -shared $(cur_dir)/jni1/test.c $(cur_dir)/jni1/test.h
	javac -d $(cur_dir) $(cur_dir)/jni1/JavaMain.java
	@echo -e "run command for test: \033[33;34m java -Djava.library.path='.' JavaMain \033[0m"

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
	@rm -f `find -name "*.run"`
	@rm -f `find -name "*.class"`
