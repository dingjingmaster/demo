java_home = $(shell find /usr/lib/jvm -name "java-*-openjdk | sort -r | head -1")
java_home = "/usr/lib/jvm/java-17-openjdk/"
java_javac = $(java_home)/bin/javac

cur_dir = $(PWD)

cflags = -I$(java_home)/include 			\
		 -I$(java_home)/include/linux

.PHONY: all clean


all:test1 test2


# 超过 open-jdk8 使用 javac -h . xxx.java 生成头文件
test1:
	gcc -o $(cur_dir)/libjni1.so $(cflags) -fPIC -shared $(cur_dir)/jni1/test.c $(cur_dir)/jni1/test.h
	$(java_javac) -d $(cur_dir) $(cur_dir)/jni1/JavaMain.java
	@echo -e "execute export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}"
	@echo -e "run command for test: \033[33;34m java -Djava.library.path='.' JavaMain \033[0m"

#test2:
	#make -C $(cur_dir)/jni3

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
	@rm -f `find -name "*.run"`
	@rm -f `find -name "*.class"`
	#@make clean -C $(cur_dir)/jni3
