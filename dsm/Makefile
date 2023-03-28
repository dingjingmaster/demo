curDir = $(realpath $(shell pwd))

all:
	g++ -D CURDIR=\""$(curDir)"\" -o dsm-test.run $(curDir)/main.cpp -L $(curDir) -laes -Wl,-rpath=$(curDir)
