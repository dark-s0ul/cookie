#pragma once

#include <functional>

template <typename T = void, template <typename> typename...>
struct property;

template <>
struct property<> {
    template <typename T>
    using get = std::function<T()>;

    template <typename T>
    using set = std::function<void(const T&)>;
};

template <typename T>
struct property<T> {
    using value_type = std::decay_t<T>;

    property(value_type& value) : m_value(value) {}
    property(value_type&& value) : m_value(std::forward<T>(value)) {}

    operator value_type&() {
        return m_value;
    }

    operator const value_type&() const {
        return m_value;
    }

    property& operator =(value_type& v) {
        m_value = v;
        return *this;
    }

    property& operator =(value_type&& v) {
        m_value = std::forward<T>(v);
        return *this;
    }
private:
    T m_value;
};

template <typename T>
struct property<T, property<>::get> {
    template <typename U>
    property(U&& get) : m_get(get) {}

    operator T() {
        return m_get();
    }

    operator const T() const {
        return m_get();
    }

private:
    const property<>::get<T> m_get;
};

template <typename T>
struct property<T, property<>::set> {
    template <typename U>
    property(U&& get) : m_set(get) {}

    property& operator =(const T& v) {
        m_set(v);
        return *this;
    }

    property& operator =(T&& v) {
        m_set(std::forward<T>(v));
        return *this;
    }

private:
    const property<>::set<T> m_set;
};

template <typename T>
struct property<T, property<>::get, property<>::set> {
    const property<>::get<T> get;
    const property<>::set<T> set;

    operator T() {
        return get();
    }

    operator const T() const {
        return get();
    }

    property& operator =(const T& v) {
        set(v);
        return *this;
    }

    property& operator =(T&& v) {
        set(std::forward<T>(v));
        return *this;
    }
};
