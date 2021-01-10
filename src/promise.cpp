#include "../include/promise.h"

namespace RunningMan
{

template<typename T>
inline Promise<T>::Promise(callback_t<T> callback)
{
  try {
    callback([this](auto&& v){_resolve(std::forward<decltype(v)>(v));}, [this](auto&& e){_reject(std::forward<decltype(e)>(e));});
  } catch (...) {
    _reject(std::current_exception());
  }
}

template <typename T>
template <typename U>
Promise<U> Promise<T>::then(std::function<U(T)> cb)
{
  return Promise<U>([this, cb](auto resolve, auto reject) {
      if (state == PromiseStates::Resolved) {
        try {
          resolve(cb(_val));
        } catch (...) {
          reject(std::current_exception());
        }
      } else if (state == PromiseStates::Pending) {
        thenHandlers.push_back([this, resolve, reject, cb](T v){
          try {
            resolve(cb(_val));
          } catch (...) {
            reject(std::current_exception());
          }
        });
      }
  });
}

template<typename T>
void Promise<T>::_resolve(T val)
{
  _val = val;
  if (state > PromiseStates::Pending) {
    return;
  }
  state = PromiseStates::Resolved;
  for (auto h : thenHandlers) {
    h(val);
  }
}

template<typename T>
void Promise<T>::_reject(const std::exception_ptr& e)
{
  // do something if we have no handlers
  if (state > PromiseStates::Pending) {
    return;
  }
  state = PromiseStates::Rejected;
  for (auto h : catchHandlers) {
    h(e);
  }
}
} // end namespace RunningMan
