#pragma once

#include <memory>

#if defined(_MSC_VER) && !defined(__MWERKS__)
#define FASTDLGT_MICROSOFT_MFP

#if !defined(__VECTOR_C)
#define FASTDLGT_HASINHERITANCE_KEYWORDS
#endif
#endif

template <typename Signature>
struct delegate;

template<typename R, typename... Args>
struct delegate<R(Args...)> {
private:
#ifdef	FASTDLGT_MICROSOFT_MFP
#ifdef FASTDLGT_HASINHERITANCE_KEYWORDS
    class __single_inheritance GenericClass {};
#else
    class GenericClass {};
#endif
#else
    class GenericClass;
#endif

public:
    using StaticFunctionPtr = auto(*)(Args...) -> R;
    using GenericMemFn = auto(GenericClass::*)(Args...) -> R;

    delegate() = default;

    explicit delegate(StaticFunctionPtr fn) {
        bind(fn);
    }

    template<class X, class Y>
    explicit delegate(Y *self, auto(X::*fn)(Args...) -> R) {
        bind(self, fn);
    }

    template<class X, class Y>
    explicit delegate(const Y *self, auto(X::*fn)(Args...) const -> R) {
        bind(self, fn);
    }

    auto operator=(StaticFunctionPtr fn) -> delegate& {
        bind(fn);
        return *this;
    }

    inline void bind(StaticFunctionPtr fn) {
        static_assert(sizeof(GenericClass *) == sizeof(StaticFunctionPtr));

        if (fn == nullptr) {
            m_func = nullptr;
        } else {
            convert(this, &delegate::InvokeStaticFunction, m_func);
        }
        m_self = horrible_cast<GenericClass*>(fn);
    }

    template<class X, class Y>
    inline void bind(const Y *self, auto(X::*fn)(Args...) -> R) = delete;

    template<class X, class Y>
    inline void bind(Y *self, auto(X::*fn)(Args...) -> R) {
        m_self = convert(implicit_cast<X *>(self), fn, m_func);
    }

    template<class X, class Y>
    inline void bind(Y *self, auto(X::*fn)(Args...) const -> R) {
        m_self = convert(const_cast<X*>(implicit_cast<const X *>(self)), fn, m_func);
    }

    template<typename... FwdArgs>
    inline auto operator()(FwdArgs &&... args) const -> R {
        return (m_self->*reinterpret_cast<GenericMemFn>(m_func))(std::forward<FwdArgs>(args)...);
    }

public:
    [[nodiscard]] inline explicit operator bool() const {
        return !empty();
    }

    [[nodiscard]] inline auto empty() const -> bool {
        return m_self == nullptr && m_func == nullptr;
    }

public:
    inline void reset() {
        m_self = nullptr;
        m_func = nullptr;
    }

private:
    template <class OutputClass, class InputClass>
    inline static auto implicit_cast(InputClass&& input) -> OutputClass {
        return std::forward<InputClass>(input);
    }

    template <class OutputClass, class InputClass>
    inline static auto horrible_cast(const InputClass input) -> OutputClass {
        union horrible_union {
            OutputClass out;
            InputClass in;
        };
        static_assert(sizeof(InputClass) == sizeof(OutputClass));
        static_assert(sizeof(InputClass) == sizeof(horrible_union));

        horrible_union u;
        u.in = input;
        return u.out;
    }

    template <class X, class XFuncType, class GenericMemFuncType>
    inline static auto convert(X *self, XFuncType fn, GenericMemFuncType &bound_func) -> GenericClass* {
        static constexpr auto SINGLE_MEMFUNCPTR_SIZE = sizeof(void (GenericClass::*)());

        if constexpr(sizeof(XFuncType) == SINGLE_MEMFUNCPTR_SIZE) {
#if defined __DMC__
            bound_func = horrible_cast<GenericMemFuncType>(fn);
#else
            bound_func = reinterpret_cast<GenericMemFuncType>(fn);
#endif
            return reinterpret_cast<GenericClass *>(self);
        }
#ifdef FASTDLGT_MICROSOFT_MFP
        else if constexpr(sizeof(XFuncType) == SINGLE_MEMFUNCPTR_SIZE + sizeof(int)) {
            union {
                XFuncType func;
                struct {
                    GenericMemFuncType funcaddress;
                    int delta;
                } s;
            } u;

            static_assert(sizeof(fn) == sizeof(u.s));
            u.func = fn;
            bound_func = u.s.funcaddress;
            return reinterpret_cast<GenericClass *>(reinterpret_cast<char *>(self) + u.s.delta);
        } else if constexpr(sizeof(XFuncType) == SINGLE_MEMFUNCPTR_SIZE + 2 * sizeof(int)) {
            struct MicrosoftVirtualMFP {
                void (GenericClass::*codeptr)();
                int delta;
                int vtable_index;
            };

            struct GenericVirtualClass : virtual public GenericClass {
                auto GetThis() -> GenericVirtualClass * { return this; }
            };

            union {
                XFuncType func;
                auto(X::*ProbeFunc)() -> GenericClass*;
                MicrosoftVirtualMFP s;
            } u;
            u.func = fn;
            bound_func = reinterpret_cast<GenericMemFuncType>(u.s.codeptr);
            union {
                auto(GenericVirtualClass::*virtfunc)() -> GenericVirtualClass*;
                MicrosoftVirtualMFP s;
            } u2;
            static_assert(sizeof(fn) == sizeof(u.s));
            static_assert(sizeof(fn) == sizeof(u.ProbeFunc));
            static_assert(sizeof(u2.virtfunc) == sizeof(u2.s));
            u2.virtfunc = &GenericVirtualClass::GetThis;
            u.s.codeptr = u2.s.codeptr;
            return (self->*u.ProbeFunc)();
        } else if constexpr (sizeof(XFuncType) == SINGLE_MEMFUNCPTR_SIZE + 3 * sizeof(int)) {
            union {
                XFuncType func;
                struct {
                    GenericMemFuncType m_funcaddress;
                    int delta;
                    int vtordisp;
                    int vtable_index;
                } s;
            } u;
            static_assert(sizeof(XFuncType) == sizeof(u.s));
            u.func = fn;
            bound_func = u.s.funcaddress;
            int virtual_delta = 0;
            if (u.s.vtable_index) {
                auto vtable = *reinterpret_cast<const int *const*>(reinterpret_cast<const char *>(self) + u.s.vtordisp );
                virtual_delta = u.s.vtordisp + *reinterpret_cast<const int *>(reinterpret_cast<const char *>(vtable) + u.s.vtable_index);
            }
            return reinterpret_cast<GenericClass *>(reinterpret_cast<char *>(self) + u.s.delta + virtual_delta);
        }
#endif
        else {
            static_assert(!std::is_same_v<XFuncType, XFuncType>);
        }
    };

    [[nodiscard]] inline auto GetStaticFunction() const -> StaticFunctionPtr {
        static_assert(sizeof(StaticFunctionPtr) == sizeof(this));
        return horrible_cast<StaticFunctionPtr>(this);
    }

    [[nodiscard]] inline auto InvokeStaticFunction(Args... args) const -> R {
        return (*GetStaticFunction())(std::forward<Args>(args)...);
    }

    GenericClass *m_self = nullptr;
    void (GenericClass::*m_func)() = nullptr;
};


template<class R, typename... Args>
delegate(auto(*)(Args...) -> R) -> delegate<R(Args...)>;

template<class X, class Y, class R, typename... Args>
delegate(Y*, auto(X::*)(Args...) -> R) -> delegate<R(Args...)>;

template<class X, class Y, class R, typename... Args>
delegate(Y*, auto(X::*)(Args...) const -> R) -> delegate<R(Args...)>;
