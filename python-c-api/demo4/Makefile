flags = \
	  -I/data/env/python2.7/include/python2.7 \
	  -L/data/env/python2.7/lib/ \
	  -lpython2.7 \
	  -Wl,-rpath,/data/env/python2.7/lib/

	  

all:demo4.run


%.run:%.c
	gcc -o $@ $(flags) $^

clean:
	@rm -f *.o
	@rm -f *.pyc
	@rm -f *.run

