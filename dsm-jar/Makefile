cflags = -w
curDir = $(shell pwd)
libName = "libencrypt-jar.so"

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target = $(strip $(patsubst %.c, %.o, $(src)))

$(libName):$(target)
	@gcc -o $@ $^ $(cflags) -fpic -shared -L$(curDir) -lfilelock `pkg-config --libs glib-2.0`
	@echo "gcc -o $@ -fpic -shared -lfilelock $^"


%.o:%.c
	@gcc -o $@ -c $< $(cflags) `pkg-config --cflags glib-2.0`

clean:
	@rm -f *.o
	@rm -f $(libName)
