all:
	gcc -shared -fPIC -c core.c
	gcc -o libcore.so core.o -shared -fPIC
	gcc demo.c -L. -lcore
	patchelf --set-rpath "\$ORIGIN" ./a.out
clean:
	@ rm -f *.o *.run *.so *.out
