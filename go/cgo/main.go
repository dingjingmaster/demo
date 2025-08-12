package main


//#cgo CFLAGS: -I./
//#cgo LDFLAGS: -L./ -lhello
//#include "hello.h"
import "C"

func main() {
	C.say_hello(C.CString("Go & c"))
}
