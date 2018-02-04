#pragma once

template<typename T>
struct function;

template<typename R, typename... U>
struct function <R(U...)> {
private:
	struct IHolder {
		virtual R invoke(U... args) = 0;
	};

	template <typename T>
	struct Holder : IHolder {
		T method;

		Holder(const T &method) : method(method) {}

		inline virtual R invoke(U... args) override {
			return method(args...);
		}
	};

	unique_ptr<IHolder> holder;

public:
	function() : holder() {}

	template <typename T>
	function(const T &method) : holder(new Holder<T>(method)) {}

	function(R(*method)(U...)) : function([=](U... args) -> R { return method(args...); }) {}

	template <class T>
	function(T *pointer, R(T::*method)(U...)) : function([=](U... args) -> R { return (pointer->*method)(args...); }) {}

	R operator()(U... args) const {
		return holder ? holder->invoke(args...) : R();
	}
};
