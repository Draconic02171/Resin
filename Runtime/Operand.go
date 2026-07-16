package runtime

import "fmt"

type Value struct {
	_type  ValueType
	_value any
}

func NewValue(value any) Value {

	switch v := value.(type) {
	case bool:
		return Value{_type: _bool_, _value: v}
	case string:
		return Value{_type: _string_, _value: v}
	case int:
		return Value{_type: _number_, _value: Number(v)}
	case int8:
		return Value{_type: _number_, _value: Number(v)}
	case int16:
		return Value{_type: _number_, _value: Number(v)}
	case int32:
		return Value{_type: _number_, _value: Number(v)}
	case int64:
		return Value{_type: _number_, _value: Number(v)}
	case float32:
		return Value{_type: _number_, _value: Number(v)}
	case float64:
		return Value{_type: _number_, _value: Number(v)}
	default:
		return Value{_type: _null_, _value: nil}
	}

}

func (v *Value) IsNull() bool {
	return v._type == _null_
}

func (v *Value) IsActuallyNull() bool {
	return v._value == nil
}

func (v *Value) GetType() ValueType { return v._type }

func (v *Value) GetValue() any { return v._value }

func (v *Value) PrintValue() string {

	Type := ConvertTypeToString(v._type)
	Value := ""

	switch v._type {
	case _null_:
		Value = fmt.Sprintf("empty (%v)", v._value)
	case _bool_:
		Value = fmt.Sprintf("%t", v._value)
	case _number_:
		Value = fmt.Sprintf("%f", v._value)
	case _string_:
		Value = fmt.Sprintf("%s", v._value)
	default:
		Value = fmt.Sprintf("%v", v._value)
	}

	return fmt.Sprintf("[ %s | %s ]", Type, Value)
}
