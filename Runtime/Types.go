package runtime

type ValueType int
type Number float64 // can be change to anytype for nummeric value

const (
	_null_ ValueType = iota
	_bool_
	_number_
	_string_
)
