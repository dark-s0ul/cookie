Light and "useful" codes for c++

## Coroutine
```cpp
#include "coroutine.h"

template <typename T>
struct generator : protected coroutine {
	using coroutine::coroutine;
	using coroutine::done;
	using coroutine::resume;

	inline T current() noexcept {
		return *static_cast<const T*>(v_ptr);
	}
};


template <typename T>
generator<T> range(T min, T max) {
	return [min, max] {
		for (T v = min; v <= max; v++) {
			yield(v);
		}
	};
}

template <typename T, typename F>
generator<int> select(generator<T>&& g, const F& predicate) {
	return [g = std::forward<generator<T>>(g), predicate]() mutable {
		while (g.resume()) {
			auto&& v = g.current();
			if (predicate(v)) {
				yield(v);
			}
		}
	};
}

bool is_power_of_2(int x) {
	return x > 0 && !(x & (x - 1));
}

int main() {
	auto s = select(range(10, 111), is_power_of_2);

	while (s.resume()) {
		std::cout << s.current() << std::endl;
	}
	
	return 0;
}

```

## Brainfuck interpreter
```cpp
#include "bf.h"

bf::eval("+++++++[>+++++++<-]>."); //print 1
```

## Getter & Setter
```cpp
#include <iostream>
#include <assert.h>

#include "property.h"

struct A {
	property<int> value {
		[&]() -> int { 
			return x * x;
		},
		[&](auto value) {
			x = value;
		}
	};

	int x;
};

int main() {
	A a;
	a.value = 10;

	assert(a.x == 10);
	assert(a.value == 100);
}
```

## Dynamic function
```cpp
#include "func_ptr.h"
#include <assert.h>

//int mul(int a, int b) { return a * b; }
auto mul = func_ptr<int(int, int)>({
	0x90, 0x01, 0x00, 0xE0,
	0x1E, 0xFF, 0x2F, 0xE1
});

int main() {
	assert(mul(23, 3) == 69);
}
```

## Function holder
```cpp
#include "function.h"

void func(int) {}

struct A { void func(int) {} } a;
struct B { void operator()(int) {} static void func(int) {} } b;

function<void(int)> f = func;
f = function<void(int)>(&a, &A::func);
f = [&](int v) -> { func(v); a.func(v); };
f = &B::func;
f = b;
```

## Custom operator
```cpp
#include "make_operator.h"
#include <assert.h>

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

int main() {
	std::vector<int> v = { 10 };

	assert(10 <in> v);
	assert(!(11 <in> v));
}
```
