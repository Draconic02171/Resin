package runtime

import (
	"errors"
	"fmt"
)

const MAXSTACK uint = 2048

type VM struct {
	running      bool //serves nothing for now
	stackPointer uint
	stackSize    uint
	stack        [MAXSTACK]Value

	funcstackPointer uint
	funcstack        [MAXSTACK]FuncFrame

	instructionPointer uint
	programSize        uint
	program            []Instruction

	memory    map[string]Value
	functions map[string]Func
}

// ----------------------Constructor---------------------- //
func InitRuntime() *VM {

	return &VM{

		running:      		false,
		stackPointer: 		0,
		stackSize:    		MAXSTACK,
		stack:        		[MAXSTACK]Value{},

		funcstackPointer: 	0,
		funcstack:        	[MAXSTACK]FuncFrame{},

		instructionPointer: 0,
		programSize:        0,
		program:            nil,

		memory: 			make(map[string]Value),
		functions: 			make(map[string]Func),

	}

}

// ----------------------  Private  ---------------------- //
func (vm *VM) incrementPointer(steps uint) error {

	if (vm.stackPointer + steps) >= vm.stackSize {
		return fmt.Errorf("Cannot increment stack pointer (stack overflow)\r\n")
	}

	vm.stackPointer += steps
	return nil

}

func (vm *VM) decrementPointer(steps uint) error {

	if vm.stackPointer < steps {
		return fmt.Errorf("Cannot decrement stack pointer (stack underflow)\r\n")
	}

	vm.stackPointer -= steps
	return nil

}

func (vm *VM) setInsPointer(where uint) error {

	if len(vm.program) < int(where) {
		return fmt.Errorf(
			"InstructionPointer out of bound! (program size: %d, tried to jump to: %d)\r\n",
			len(vm.program),
			int(where),
		)
	}

	vm.instructionPointer = where
	return nil

}

func (vm *VM) getInsPointer() uint { return vm.instructionPointer }

func (vm *VM) getCurrentInst() *Instruction { return &vm.program[vm.instructionPointer] }

// --------------------Getter / Setter-------------------- //
func (vm *VM) IsRunning() bool { return vm.running } // not in use for now

// ----------------------  Public  ---------------------- //
func (vm *VM) LoadProgram(program []Instruction) error {

	if program == nil {
		return fmt.Errorf("Cannot load the program (nothing passed )")
	}
	if len(program) == 0 {
		return fmt.Errorf("Theres no program to load (program size = 0)")
	}

	vm.program = program
	vm.programSize = uint(len(program))

	for i := range len(vm.program) {

		Operands := vm.program[i].operand
		
		if vm.program[i].opcode == Op_Function {
			if Operands[0].IsNull() || Operands[0]._type != _string_ {
				return fmt.Errorf("Failed to Compile: function declaration require name as a string literal\r\n")	
			}

			FuncName := Operands[0]._value.(string)
			Metadata := Func{
				name: FuncName,
				entry: uint(i),
			}

			vm.functions[FuncName] = Metadata

 		}
	}

	return nil

}

// run the loaded program
func (vm *VM) Run() error {

	var Msg any = nil // for catching an opcode error or warning
	RuntimeErrorMsg := "Runtime Error!"
	vm.running = true

	vm.__call__([]Value{NewValue("main")})

	for vm.running {

		if vm.instructionPointer >= vm.programSize {
			vm.running = false
			return errors.Join(
				errors.New(RuntimeErrorMsg),
				fmt.Errorf("Insturction Pointer exceeds the program size for some reason\r\n"),
			)
		}

		if vm.funcstackPointer <= 0 {vm.running = false}

		CurrentIns := vm.program[vm.instructionPointer]

		switch CurrentIns.opcode {
		case Op_Push:
			Msg = vm.__push__(CurrentIns.operand)
		case Op_Pop:
			Msg, _ = vm.__pop__()
		case Op_Swap:
			Msg = vm.__swap__()
		case Op_Dup:
			Msg = vm.__dup__()
		case Op_Load:
			Msg = vm.__load__(CurrentIns.operand)
		case Op_Store:
			Msg = vm.__store__(CurrentIns.operand)
		case Op_Add:
			Msg = vm.__add__()
		case Op_Sub:
			Msg = vm.__sub__()
		case Op_Mul:
			Msg = vm.__mul__()
		case Op_Div:
			Msg = vm.__div__()
		case Op_Mod:
			Msg = vm.__mod__()
		case Op_Equal:
			Msg = vm.__equal__()
		case Op_More:
			Msg = vm.__more__()
		case Op_Less:
			Msg = vm.__less__()
		case Op_MoreOrEqual:
			Msg = vm.__moreOrEqual__()
		case Op_LessOrEqual:
			Msg = vm.__lessOrEqual__()
		case Op_Jump: //Msg 
		case Op_JumpIfTrue:
		case Op_JumpIfFalse:
		case Op_Function: //nothing
		case Op_Call: vm.__call__(CurrentIns.operand)
		case Op_Return: vm.__return__(CurrentIns.operand)
		case Op_Read:
		case Op_Print:
		case Op_Peek:
			Msg = vm.__peek__()
		case Op_Halt:
			vm.running = false
		}

		if Msg != nil {
			fmt.Printf("%v", Msg)
		}
		vm.setInsPointer(vm.getInsPointer() + 1)
	}

	return nil
}
