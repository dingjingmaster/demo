package main


//#cgo CFLAGS: -I./
//#cgo LDFLAGS: -L./ -lhello
//#include "hello.h"
import "C"
import "fmt"

type Point struct {
	X int
	Y int
}

func main() {
	C.say_hello(C.CString("Go & c"))

	addRes := C.add(3, 5)
	goRes := int (addRes)
	fmt.Printf("Result of add(3, 5): %d\n", goRes)

	cp := C.create_point(11, 12);
	if nil == cp {
		fmt.Printf("Failed to create point.\n")
		return
	}

	goPoint := Point {
		X: int (cp.x),
		Y: int (cp.y),
	}
	C.free_point(cp)
	fmt.Printf("Point: (%d, %d)\n", goPoint.X, goPoint.Y)
}
