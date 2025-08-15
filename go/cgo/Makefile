all:libhello.a main.run goToC.run

goToC.run:libmylib.a
	gcc -fPIE -o $@ goToC.c -L . -lmylib

libmylib.a:mylib.go
	go build -o $@ -buildmode=c-archive -gcflags=-shared $<
	#go build -o $@ -buildmode=c-shared $<

main.run:main.go
	go build -o $@ $<

libhello.a:hello.o
	ar rcs $@ $^

hello.o:hello.c
	gcc -fPIC -c $^ -o $@

clean:
	@rm -f *.so *.o *.a *.run main
	@echo 'done!'
