Light and "useful" codes for c++

## Brainfuck interpreter
```
#include "bf.h"

bf::eval("+++++++[>+++++++<-]>."); //print 1
```

## Dynamic function
```
#include "func_ptr.h"

//int mul(int a, int b) { return a * b; }
auto mul = func_ptr<int(int, int)>({
	0x90, 0x01, 0x00, 0xE0,
	0x1E, 0xFF, 0x2F, 0xE1
});

int res = mul(23, 3); //69
```

## Function holder
```
#include "function.h"

void func(int) {}

struct A { void func(int) {} } a;
struct B { void operator()(int) {} } b;

function<void(int)> f = func;
f(0); // call func(0);

f = function<void(int)>(&a, &A::func);
f(0); // call a->func(0);

f = [&](int v) -> { func(v); a->func(v); };
f(0); // call lambda

f = b;
f(0); // call b.operator()(0);
```

## Custom operator
```
#include "make_operator.h"

//Using struct
struct _in {
	template <typename T>
	inline bool operator ()(T value, std::vector<T> const& vector) const {
		return std::find(vector.begin(), vector.end(), value) != vector.end();
	}
};
auto in = make_operator(_in());

//Using lambda
auto in = make_operator([](auto value, auto vector) -> bool {
	return std::find(vector.begin(), vector.end(), value) != vector.end();
});

std::vector<int> v = { 10 };

bool res = 10 <in> v; //true
res = 11 <in> v; //false
```
