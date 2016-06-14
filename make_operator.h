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
	inline value<V, F> operator < (V left, wrapper<F> right) {
		return{ left , right.ptr };
	}

	template<typename V, typename F, typename T>
	inline auto operator > (value<V, F> left, T right) {
		return left.ptr(left.val, right);
	}

	template<typename P>
	inline constexpr wrapper<P> make_operator(P p) {
		return{ p };
	}
}
#endif 
