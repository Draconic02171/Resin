package runtime

import (
	"errors"
	"fmt"
)

type OpcodeType uint

const ( //Resin Arch
	_Invalid OpcodeType = iota
	Op_Push
	Op_Pop
	Op_Swap
	Op_Dup
	Op_Load
	Op_Store

	Op_Add
	Op_Sub
	Op_Mul
	Op_Div
	Op_Mod

	Op_Equal
	Op_More
	Op_Less
	Op_MoreOrEqual
	Op_LessOrEqual

	Op_Jump
	Op_JumpIfTrue
	Op_JumpIfFalse

	Op_Function // marker
	Op_Call
	Op_Return

	Op_Read
	Op_Print
	Op_Peek
	Op_Halt //not sure what to do with this
)

func (vm *VM) __push__(operand []Value) error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'push'"
	if len(operand) == 0 {
		return fmt.Errorf("%s \r\nno argument passed (operand count: %d)\r\n", errMsg, len(operand))
	}

	err := vm.incrementPointer(1)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	vm.stack[vm.stackPointer] = operand[0]

	return nil

}
func (vm *VM) __pop__() (error, Value) {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'pop'"

	ReturnValue := vm.stack[vm.stackPointer]
	vm.stack[vm.stackPointer] = Value{}
	err := vm.decrementPointer(1)

	if err != nil {
		return errors.Join(errors.New(errMsg), err), Value{}
	}

	return nil, ReturnValue

}
func (vm *VM) __swap__() error {
	errMsg := "Error: 'swap'"

	TopValue := vm.stack[vm.stackPointer]

	err := vm.decrementPointer(1)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	BottomValue := vm.stack[vm.stackPointer]
	vm.stack[vm.stackPointer] = TopValue

	err = vm.incrementPointer(1)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	vm.stack[vm.stackPointer] = BottomValue

	return nil
}
func (vm *VM) __dup__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'dup'"
	CopiedValue := vm.stack[vm.stackPointer]

	err := vm.incrementPointer(1)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	vm.stack[vm.stackPointer] = CopiedValue

	return nil

}
func (vm *VM) __load__(operand []Value) error { //not test yet!

	errMsg := "Error: 'load'"
	if len(operand) == 0 {
		return fmt.Errorf(
			"%s \r\nno argument passed (operand count: %d)",
			errMsg,
			len(operand),
		)
	}
	if operand[0]._type != _string_ {
		return fmt.Errorf(
			"%s \r\nrequire string type argument",
			errMsg,
		)
	}

	VarName := operand[0]._value.(string)
	LoadedValue, success := vm.memory[VarName]

	if !success {
		return fmt.Errorf(
			"%s \r\nTrying to access to a non-declared variable: %s",
			errMsg,
			VarName,
		)
	}

	err := vm.__push__([]Value{LoadedValue})
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil

}
func (vm *VM) __store__(operand []Value) error { //not test yet!

	errMsg := "Error: 'store'"
	if len(operand) <= 0 {
		return fmt.Errorf("%s \r\nno argument passed (operand count: %d)", errMsg, len(operand))
	}
	if operand[0]._type != _string_ {
		return fmt.Errorf("%s \r\nrequire string type argument", errMsg)
	}

	VarName := operand[0]._value.(string)
	StoredValue := vm.stack[vm.stackPointer]

	if StoredValue._type == _null_ {
		return fmt.Errorf("%s \r\ntrying to store a null value", errMsg)
	}

	vm.memory[VarName] = StoredValue

	return nil

}

func (vm *VM) __add__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'add'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _number_, _value: left + right}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __sub__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'sub'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _number_, _value: left - right}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __mul__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'mul'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _number_, _value: left * right}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __div__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'div'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _number_, _value: left / right}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __mod__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'mod'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	// convert back to int for modulo
	left := int(Lefthand.GetValue().(Number))
	right := int(RightHand.GetValue().(Number))

	ReturnValue := []Value{{_type: _number_, _value: Number(left % right)}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}

func (vm *VM) __equal__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'equal'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _bool_, _value: bool(left == right)}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __more__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'more'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _bool_, _value: bool(left > right)}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __less__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'less'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _bool_, _value: bool(left < right)}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __moreOrEqual__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'moreOrEqual'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _bool_, _value: bool(left >= right)}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}
func (vm *VM) __lessOrEqual__() error {

	//write an error handling for insPointer out of bound
	errMsg := "Error: 'lessOrEqual'"

	err, RightHand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	err, Lefthand := vm.__pop__()
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	{ // check for value type
		if RightHand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
		if Lefthand.GetType() != _number_ {
			return errors.Join(
				errors.New(errMsg),
				errors.New("right hand side is not a number"),
			)
		}
	}

	left := Lefthand.GetValue().(Number)
	right := RightHand.GetValue().(Number)

	ReturnValue := []Value{{_type: _bool_, _value: bool(left <= right)}}

	err = vm.__push__(ReturnValue)
	if err != nil {
		return errors.Join(errors.New(errMsg), err)
	}

	return nil
}

func (vm *VM) __call__(operand []Value) error {

	errMsg := "Error: 'call'"
	if len(operand) == 0 || operand[0].IsActuallyNull() {
		return fmt.Errorf("%s first operand is null\r\n", errMsg)
	}
	if operand[0]._type != _string_ {
		return fmt.Errorf("%s operand is not a string\r\n", errMsg)
	}

	FuncName := operand[0]._value.(string)
	FuncData, success := vm.functions[FuncName]

	if !success {
		return fmt.Errorf(
			"%s Theres no function name '%s' loaded\r\n",
			errMsg,
			FuncName,
		)
	}

	ReturnPoint := FuncFrame{
		LastInstPointer:  vm.instructionPointer,
		LaststackPointer: vm.stackPointer,
	}

	vm.funcstackPointer++
	vm.funcstack[vm.funcstackPointer] = ReturnPoint

	vm.setInsPointer(FuncData.entry)

	return nil
}

func (vm *VM) __return__(operand []Value) error {

	errMsg := "Error: 'return'"
	if len(operand) == 0 || operand[0].IsActuallyNull() {
		return fmt.Errorf("%s first operand is null\r\n", errMsg)
	}
	if operand[0]._type != _number_ {
		return fmt.Errorf("%s operand is not a number\r\n", errMsg)
	}
	if vm.funcstackPointer == 0 {
		return fmt.Errorf("%s FuncStack is Empty\r\n", errMsg)
	}

	vm.stackPointer = vm.funcstack[vm.funcstackPointer].LaststackPointer
	vm.instructionPointer = vm.funcstack[vm.funcstackPointer].LastInstPointer
	vm.funcstack[vm.funcstackPointer] = FuncFrame{}
	vm.funcstackPointer--

	return nil
}

func (vm *VM) __peek__() error {

	fmt.Print(vm.stack[vm.stackPointer].GetValue())
	return nil
}
