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
				Runtime.NewValue("Return5"),
				Runtime.NewValue(0),
			},
			0,
		),
		Runtime.Code(Runtime.Op_Push, []Runtime.Value{Runtime.NewValue(5)}, 1),
		Runtime.Code(Runtime.Op_Return, []Runtime.Value{Runtime.NewValue(1)}, 2),

		Runtime.Code(
			Runtime.Op_Function,
			[]Runtime.Value{
				Runtime.NewValue("main"),
				Runtime.NewValue(0),
			}, // argument amount, maybe
			3,
		),
		Runtime.Code(Runtime.Op_Call, []Runtime.Value{Runtime.NewValue("Return5")}, 4),
		Runtime.Code(Runtime.Op_Call, []Runtime.Value{Runtime.NewValue("Return5")}, 5),
		Runtime.Code(Runtime.Op_Add, []Runtime.Value{}, 6),
		Runtime.Code(Runtime.Op_Peek, []Runtime.Value{}, 7),
		Runtime.Code(Runtime.Op_Peek, []Runtime.Value{}, 8),
		Runtime.Code(Runtime.Op_Halt, []Runtime.Value{}, 9), //exit program
	}

	vm := Runtime.InitRuntime()
	vm.LoadProgram(Program)
	vm.Run()
	// vm.DumpCore()
}
