package runtime

type Instruction struct {
	opcode       OpcodeType
	operand      []Value
	originalLine uint
}

func Code(Opcode OpcodeType, Operand []Value, Line uint) Instruction {
	return Instruction{opcode: Opcode, operand: Operand, originalLine: Line}
}
func (ins *Instruction) GetOpcode() OpcodeType { return ins.opcode }
func (ins *Instruction) GetOperand() []Value   { return ins.operand }
func (ins *Instruction) GetCodeLine() uint     { return ins.originalLine }
