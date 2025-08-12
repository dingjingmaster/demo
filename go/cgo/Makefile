all:libhello.a main.run

main.run:main.go
	go build $<

libhello.a:hello.o
	ar rcs $@ $^

hello.o:hello.c
	gcc -fPIC -c $^ -o $@

clean:
	@rm -f *.o *.a *.run main
	@echo 'done!'
