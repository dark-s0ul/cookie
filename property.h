#pragma once

#include <functional>

template <typename...>
struct property;

template <>
struct property<> {
    using get = struct {};
    using set = struct {};
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
    const std::function<T()> m_get;
};

template <typename T>
struct property<T, property<>::set> {
    using value_type = std::decay_t<T>;

    template <typename U>
    property(U&& get) : m_set(get) {}

    property& operator =(value_type& v) {
        set(v);
        return *this;
    }

    property& operator =(value_type&& v) {
        set(std::forward<T>(v));
        return *this;
    }

private:
    const std::function<void(const T&)> m_set;
};

template <typename T>
struct property<T, property<>::get, property<>::set> {
    using value_type = std::decay_t<T>;

    const std::function<T()> get;
    const std::function<void(const T&)> set;

    operator T() {
        return get();
    }

    operator const T() const {
        return get();
    }

    property& operator =(value_type& v) {
        set(v);
        return *this;
    }

    property& operator =(value_type&& v) {
        set(std::forward<T>(v));
        return *this;
    }
};
