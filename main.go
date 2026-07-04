package main

import "fmt"
import "os/exec"

func main() {
	kernel_start := exec.Command("./kernel/kernel")
	out, _ := kernel_start.Output()
	fmt.Println(string(out))
}
