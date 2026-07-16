package runtime

func ConvertOpcodeToString(Opcode OpcodeType) string {
	switch Opcode {
	case Op_Push:
		return "Push"
	case Op_Pop:
		return "Pop"
	case Op_Swap:
		return "Swap"
	case Op_Dup:
		return "Dup"
	case Op_Load:
		return "Load"
	case Op_Store:
		return "Store"

	case Op_Add:
		return "Add"
	case Op_Sub:
		return "Sub"
	case Op_Mul:
		return "Mul"
	case Op_Div:
		return "Div"
	case Op_Mod:
		return "Mod"

	case Op_Equal:
		return "Equal"
	case Op_More:
		return "More"
	case Op_Less:
		return "Less"
	case Op_MoreOrEqual:
		return "MoreOrEqual"
	case Op_LessOrEqual:
		return "LessOrEqual"

	case Op_Jump:
		return "Jump"
	case Op_JumpIfTrue:
		return "JumpIfTure"
	case Op_JumpIfFalse:
		return "JumpIfFalse"

	case Op_Function:
		return "Function"
	case Op_Call:
		return "Call"
	case Op_Return:
		return "Return"

	case Op_Read:
		return "Read"
	case Op_Print:
		return "Print"
	case Op_Peek:
		return "Peek"
	case Op_Halt:
		return "Halt"
	}
	return "Unkown"
}

func ConvertTypeToString(Type ValueType) string {
	switch Type {
	case _null_:
		return "null"
	case _bool_:
		return "bool"
	case _number_:
		return "numeric"
	case _string_:
		return "string"
	}
	return "Unkown"
}
