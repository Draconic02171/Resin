package runtime

import "fmt"

// this is for Dumping the program stack, memory layout and runtime attribute at call time//
func (vm *VM) DumpCore() {
	fmt.Println()
	fmt.Println("===== CORE DUMP =====")
	fmt.Printf("StackPointer: %d\r\n" , vm.stackPointer)
	fmt.Printf("InstPointer : %d\r\n" , vm.instructionPointer)
	for i := uint(1); i <= vm.stackPointer; i++ {  
    	fmt.Println(vm.stack[i].PrintValue())
	}
}
