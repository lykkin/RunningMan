#pragma once

#include <functional>
#include <exception>
#include <vector>

namespace RunningMan
{

enum class PromiseStates {
  Pending,
  Resolved,
  Rejected
};

template<typename T>
using resolve_cb_t = std::function<void(T)>;
typedef std::function<void(const std::exception_ptr&)> reject_cb_t;

template<typename T>
using callback_t = std::function<void(resolve_cb_t<T>, reject_cb_t)>;

template <typename T>
class Promise
{
public:
  inline Promise(callback_t<T> callback);

  template <typename U>
  Promise<U> then(std::function<U(T)> cb);

private:
  T _val;
  PromiseStates state = PromiseStates::Pending;
  std::vector<resolve_cb_t<T>> thenHandlers;
  std::vector<reject_cb_t> catchHandlers;
  void _resolve(T val);
  void _reject(const std::exception_ptr& e);
};
} // end namespace Promise
