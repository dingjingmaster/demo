package main

import "fmt"

func main() {
	var l = make([]byte, 128, 256)
	fmt.Println("l: ", len(l), cap(l))
	fmt.Printf("%s -- %c -- %d\n", l, l[0], l[0])

	for i := range 128 {
		l[i] = 'A'
	}
	fmt.Println(l)
	fmt.Printf("%s\n", l)
}
