#pragma once

typedef std::function<void(const std::exception_ptr&)> reject_cb_t;
typedef std::function<void()> void_resolve_cb_t;
typedef std::function<void(void_resolve_cb_t, reject_cb_t)> void_callback_t;

template<typename T>
using resolve_cb_t = std::function<void(T)>;

template<typename T>
using callback_t = std::function<void(resolve_cb_t<T>, reject_cb_t)>;

