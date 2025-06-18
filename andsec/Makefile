all = test-html-tooltip.run

.PHONY:$(all) clean

$(all):


test-html-tooltip.run:test-html-tooltip.c ipc.c
	gcc -o $@ $^ `pkg-config --cflags --libs gio-2.0`

clean:
	@rm -f *.run
