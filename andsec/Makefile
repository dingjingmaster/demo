targets = test-html-tooltip.run test-update-policy.run

.PHONY:all clean

all:$(targets)


%.run:%.c ipc.c
	gcc -o $@ $^ `pkg-config --cflags --libs gio-2.0`

#test-update-policy.run:test-update-policy.c ipc.c
#	gcc -o $@ $^ `pkg-config --cflags --libs gio-2.0`

clean:
	@rm -f *.run
