#pragma once

#ifndef MAKE_OPERATOR
#define MAKE_OPERATOR

namespace{
  template<typename P>
	struct wrapper {
		P ptr;
	};

	template<typename V, typename F>
	struct value {
		V val;
		F ptr;
	};

	template<typename V, typename F>
	inline value<V, F> operator < (V v, wrapper<F> o) {
		return{ v , o.ptr };
	}

	template<typename F, typename V, typename T>
	inline auto operator > (value<F, V> a, T b) {
		return a.ptr(a.val, b);
	}

	template<typename F>
	inline constexpr wrapper<F> make_operator(F f) {
		return{ f };
	}
}
#endif 
