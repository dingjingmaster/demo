cflags = -w -g -Wl,-rpath='$$ORIGIN'
curDir = $(shell pwd)
libName = "libencrypt-jar.so"
javac=$(shell $(find /usr/lib/jvm/java*/bin/ -name javac))

src = $(curDir)/encrypt-in-block-for-jar.c #$(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target = $(strip $(patsubst %.c, %.o, $(src)))

$(libName):$(src)
	@gcc -shared -fpic -o $@ $^ $(cflags) -L$(curDir) -lfilelock -luuid
	@echo "gcc -o $@ -fpic -shared -lfilelock $^"


%.o:%.c
	@gcc -o $@ -c $< $(cflags)

clean:
	@rm -f *.o
	@rm -f $(libName)
