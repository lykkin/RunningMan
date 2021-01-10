#include "../include/promise.h"
#include "./promise_engine.cpp"

namespace RunningMan
{

template<typename T>
inline Promise<T>::Promise(callback_t<T> callback)
{
  try {
    callback([this](auto v){
      PromiseEngine::enqueue([this, v]() {
        //_resolve(std::forward<decltype(v)>(v));
        _resolve(v);
      });
    }, [this](auto&& e){
      PromiseEngine::enqueue([this, e]() {
        //_reject(std::forward<decltype(e)>(e));
        _reject(e);
      });
    });
  } catch (...) {
    _reject(std::current_exception());
  }
}

template <typename T>
template <typename U>
Promise<U>* Promise<T>::then(std::function<U(T)> cb)
{
  return new Promise<U>([this, cb](auto resolve, auto reject) {
      if (state == PromiseStates::Fulfilled) {
        try {
          resolve(cb(value));
        } catch (...) {
          reject(std::current_exception());
        }
      } else if (state == PromiseStates::Pending) {
        thenHandlers.push_back([this, resolve, reject, cb](T v){
          try {
            resolve(cb(value));
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
  if (state > PromiseStates::Pending) {
    return;
  }
  value = val;
  state = PromiseStates::Fulfilled;
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
