package main

import (
	"errors"
	"fmt"
	"os"
	"strconv"
	"path/filepath"
)

func GetProcName(pid int) (string, error) {
	data, err := os.Readlink(fmt.Sprintf("/proc/%d/exe", pid))
	if err != nil {
		return "", errors.New(fmt.Sprintf("Readlink err: %v", err))
	}
	return filepath.Base(data), nil
}

func main () {
	if len(os.Args) < 2 {
		fmt.Println("...\n");
		return
	}
	i, _ := strconv.Atoi(os.Args[1])
	f, _ := GetProcName(i)
	fmt.Printf("%s\n", f)
}
