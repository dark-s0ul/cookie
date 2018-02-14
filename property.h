#pragma once

template <typename T>
struct property {
private:
	template<typename R>
	struct accessor;

	template<typename R, typename... U>
	struct accessor <R(U...)> {
	private:
		struct method {
			virtual R invoke(U... args) = 0;
		};

		template <typename M>
		struct holder : method {
			M func;

			inline holder(const M &func) : func(func) {}

			inline virtual R invoke(U... args) override {
				return func(args...);
			}
		};

		method *func;

	public:
		template <typename M>
		inline accessor(const M &func) : func(new holder<M>(func)) {}

		inline ~accessor() {
			std::cout << func << std::endl;
			delete func;
			func = 0;
		}

		inline R operator()(U... args) const {
			return func->invoke(args...);
		}
	};

	accessor<T()> get;
	accessor<void(const T&)> set;

public:
	inline property(const auto &get, const auto& set) : get(get), set(set) {}

	inline property(property&&) = delete;

	inline property<T>& operator=(const property<T>& prop) {
		return *this;
	}

	inline property<T>& operator=(const T& value) {
		set(value);
		return *this;
	}

	inline operator T() {
		return get();
	}

	inline operator const T() const {
		return get();
	}
};
