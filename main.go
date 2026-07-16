package main

import (
	Runtime "Resin/Runtime"
)

func main() {

	//Runtime.Code(<opcode> , []value{} , <originalLine, for debugging purpose>)
	Program := []Runtime.Instruction{

		Runtime.Code(
			Runtime.Op_Function,
			[]Runtime.Value{
				Runtime.NewValue("main"),
				Runtime.NewValue(2)}, // argument amount, maybe
			0,
		),
		Runtime.Code(Runtime.Op_Push, 	[]Runtime.Value{Runtime.NewValue(5)}, 0),
		Runtime.Code(Runtime.Op_Push, 	[]Runtime.Value{Runtime.NewValue(5)}, 0),
		Runtime.Code(Runtime.Op_Add, 	[]Runtime.Value{}, 0),                       // add those 2 arguments
		Runtime.Code(Runtime.Op_Peek, 	[]Runtime.Value{}, 0),
		Runtime.Code(Runtime.Op_Return, []Runtime.Value{Runtime.NewValue(1)}, 0), //return 1 value
		Runtime.Code(Runtime.Op_Return, []Runtime.Value{}, 0),
	}

	vm := Runtime.InitRuntime()
	vm.LoadProgram(Program)
	vm.Run()
	vm.DumpCore()
}
