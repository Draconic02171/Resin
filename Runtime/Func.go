package runtime

type Func struct {
	name  string
	entry uint
}

func (vm *VM) CreateFunc(Name string, Entry uint) Func {
	return Func{name: Name, entry: Entry}
}

func (fn Func) GetEntry() uint { return fn.entry }
