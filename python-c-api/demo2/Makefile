flags = \
	  -I/data/env/python/include/python3.10 \
	  -L/data/env/python/lib/libpython3.10.a \
	  -lpython3.10

all:demo2.run


%.run:%.c
	gcc -o $@ $(flags) $^

clean:
	@rm -f *.o
	@rm -f *.run

