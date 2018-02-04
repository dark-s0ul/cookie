#pragma once

template<typename T>
struct function;

template<typename R, typename... U>
struct function <R(U...)> {
private:
	struct IHolder {
		int refs;
		virtual R invoke(U... args) = 0;
	};

	template <typename T>
	struct Holder : IHolder {
		T method;

		inline Holder(const T &method) : method(method) {
			IHolder::refs = 1;
		}

		inline virtual R invoke(U... args) override {
			return method(args...);
		}
	};

	IHolder *holder;

public:
	inline function() : holder() {}

	inline function(const function<R, U...> &f) {
		holder = f.holder;
		if (holder) {
			holder->refs++;
		}
	}

	inline function <R, U...> &operator =(const function<R, U...> &f) {
		if (holder && --holder->refs == 0) {
			delete holder;
		}
		holder = f.holder;
		if (holder) {
			holder->refs++;
		}
		return *this;
	}

	template <typename T>
	inline function(const T &method) : holder(new Holder<T>(method)) {}

	inline function(R(*method)(U...)) : function([=](U... args) -> R { return method(args...); }) {}

	template <class T>
	inline function(T *pointer, R(T::*method)(U...)) : function([=](U... args) -> R { return (pointer->*method)(args...); }) {}

	inline ~function() {
		if (--holder->refs == 0) {
			delete holder;
		}
	}

	inline R operator()(U... args) const {
		return holder ? holder->invoke(args...) : R();
	}
};
