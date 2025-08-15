package main

import "C"
import "fmt"

//export SayHello
func SayHello(name *C.char) {
	goName := C.GoString(name)
	fmt.Println("C <-- Go, ", goName)
}

func main() {}
