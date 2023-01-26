Light and "useful" codes for c++

## Coroutine
```cpp
#include "coroutine.h"

template <typename T>
struct generator : protected coroutine {
	using coroutine::coroutine;
	using coroutine::done;
	using coroutine::resume;
	
	struct iterator {
		generator* g;
		bool m_resume;

		inline bool operator!=(iterator& other) noexcept {
			return (g == other.g) && (m_resume != other.m_resume);
		}

		inline void operator++() noexcept {
			m_resume = g->resume();
		}

		inline T operator*() noexcept {
			return g->current();
		}
	};
	
	[[nodiscard]] inline iterator begin() noexcept {
		return {this, resume()};
	}

	[[nodiscard]] inline iterator end() noexcept {
		return {this, false};
	}

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
generator<int> select(T&& g, const F& predicate) {
	return [g = std::forward<T>(g), predicate]() mutable {
		for (auto v : g) {
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
	for (auto v : select(range(10, 111), is_power_of_2)) {
		std::cout << v << std::endl;
	}
	
	return 0;
}

```

## Brainfuck interpreter
```cpp
#include "bf.h"

bf::eval("+++++++[>+++++++<-]>."); //print 1
```

## Property
```cpp
#include <iostream>
#include <assert.h>

#include "property.h"

int value = 0;
property<int, property<>::get, property<>::set> get_set_value = {
	.get = [] { return value; },
	.set = [] (const int v) {
		value = v * v;
	}
};
property<int, property<>::get> only_get_value = [] { 
	return value; 
};

property<int, property<>::set> only_set_value = [](const int v) { 
	value = v * v; 
};

property<int&> ref_to_value = value;
property<int> other_value = value;
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

## Meta function
```cpp
#include "meta_function.hpp"

int main() {
    constexpr auto fn1 = $fn((a, b, c, d) -> d - ((a - b) + c));
    constexpr auto fn2 = [](int a, int b, int c, int d) {return d - ((a - b) + c); };

    static_assert(fn1(12, 97, 11, 121) == fn2(12, 97, 11, 121));

    constexpr auto p_fn = static_cast<int(*)(int, int, int, int)>(fn1);

    static_assert(195 == p_fn(12, 97, 11, 121));
    return 0;
}
```
