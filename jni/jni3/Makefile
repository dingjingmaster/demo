### java 例子
javaHome = "/usr/lib/jvm/java-17-openjdk/"
javaJavac = $(javaHome)/bin/javac
###
cflags = -w -g -Wl,-rpath='$$ORIGIN'
curDir = $(shell pwd)
libName = "libencrypt-jar.so"
javac=$(shell $(find /usr/lib/jvm/java*/bin/ -name javac))

src = $(curDir)/encrypt-in-block-for-jar.c $(curDir)/encrypt-jni.c #$(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target = $(strip $(patsubst %.c, %.o, $(src)))

$(libName):$(src)
	@$(javaJavac) -h $(curDir)/test $(curDir)/test/Main.java \
		&& mv $(curDir)/test/Main.h $(curDir)/test.h
	@gcc -shared -fpic -o $@ $^ $(cflags) -I$(javaHome)/include -I$(javaHome)/include/linux -L$(curDir) -lfilelock -luuid
	@echo "gcc -o $@ -fpic -shared -lfilelock $^"


%.o:%.c
	@gcc -o $@ -c $< $(cflags)

clean:
	@rm -f *.o
	@rm -f $(libName)
	@rm -f `find -name "*.class"`
