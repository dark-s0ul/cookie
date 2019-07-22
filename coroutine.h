#pragma once

#include <ucontext.h>
#include <signal.h>
#include <utility>
#include <cassert>

#include <iostream>

struct coroutine {
	coroutine(const coroutine& c) = delete;
	coroutine(coroutine&& c) noexcept
		: m_data(c.m_data)
		, m_start(c.m_start)
		, m_dtor(c.m_dtor)
	{
		c.m_data = nullptr;

		ctx.uc_link = &link;
		ctx.uc_stack.ss_sp = c.ctx.uc_stack.ss_sp;
		ctx.uc_stack.ss_size = c.ctx.uc_stack.ss_size;
	}

	coroutine& operator=(const coroutine& c) = delete;
	coroutine& operator=(coroutine&& c) = delete;

	template <typename F>
	coroutine(F&& func, size_t ss_size = SIGSTKSZ) noexcept
			: m_data(new char[ss_size + sizeof(F)])
			, m_start((start_t) &coroutine::start<F>)
			, m_dtor((dtor_t) &coroutine::dtor<F>)
	{
		new (m_data) F(std::forward<F>(func));

		ctx.uc_link = &link;
		ctx.uc_stack.ss_sp = m_data + sizeof(F);
		ctx.uc_stack.ss_size = ss_size;
	}

	~coroutine() noexcept {
		if (m_data != nullptr) {
			m_dtor(m_data);

			delete[] m_data;
			m_data = nullptr;
		}
	}

	inline bool resume() noexcept {
		if (!m_init) {
			m_init = true;
			getcontext(&ctx);
			makecontext(&ctx, m_start, 2, m_data, this);
		}
		if (!m_done) {
			auto co = this_coro;
			this_coro = this;
			swapcontext(ctx.uc_link, &ctx);
			this_coro = co;

			return !m_done;
		}
		return false;
	}

	inline bool done() noexcept {
		return m_done;
	}

protected:
	thread_local static coroutine* this_coro;

	typedef void(*start_t)();
	typedef void(*dtor_t)(void*);

	char* m_data;
	start_t m_start;
	dtor_t m_dtor;
	ucontext_t link, ctx;
	bool m_init = false;
	bool m_done = false;
	const void* v_ptr;

	template <typename T>
	static void start(T *fn, coroutine *co) noexcept {
		assert(fn != nullptr);

		fn->operator()();
		co->m_done = true;
	}

	template <typename T>
	static void dtor(T* p) {
		p->~T();
	}

	friend void yield() noexcept;

	template <typename T>
	friend void yield(T&& v) noexcept;
};

inline void yield() noexcept {
	swapcontext(&coroutine::this_coro->ctx, coroutine::this_coro->ctx.uc_link);
}

template <typename T>
inline void yield(T&& v) noexcept {
	coroutine::this_coro->v_ptr = std::addressof(v);
	yield();
}

inline static void await(coroutine&& co) noexcept {
	while (co.resume()) { yield(); }
}
