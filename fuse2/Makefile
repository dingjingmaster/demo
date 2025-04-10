.PHONY:all clean

cflags = -D_FILE_OFFSET_BITS=64 `pkg-config --cflags fuse`

all: fuse21-example.run fuse25-example.run

fuse21-example.run:fuse21.c
	@gcc $(cflags) -o $@ $^ `pkg-config --libs fuse`
	@echo CC -o $@ $^

fuse25-example.run:fuse25.c
	@gcc $(cflags) -o $@ $^ `pkg-config --libs fuse`
	@echo CC -o $@ $^

clean:
	@rm -f *.run
