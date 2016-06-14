#pragma once

#ifndef MAKE_OPERATOR
#define MAKE_OPERATOR

namespace{
	template<typename P>
	struct wrapper {
		P ptr;
	};

	template<typename V, typename P>
	struct value {
		V val;
		P ptr;
	};

	template<typename V, typename P>
	inline value<V, P> operator < (V left, wrapper<P> right) {
		return{ left , right.ptr };
	}

	template<typename V, typename P, typename T>
	inline auto operator > (value<V, P> left, T right) {
		return left.ptr(left.val, right);
	}

	template<typename P>
	inline constexpr wrapper<P> make_operator(P p) {
		return{ p };
	}
}
#endif 
