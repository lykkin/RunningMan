#pragma once

#include <functional>
#include <memory>
#include <exception>
#include <vector>

#include "running-man/promise_engine.h"

namespace RunningMan
{
static int promise_id = 0;

enum class PromiseStates {
  Pending,
  Fulfilled,
  Rejected
};

typedef std::function<void(const std::exception_ptr&)> reject_cb_t;

template<typename T>
using resolve_cb_t = std::function<void(T)>;

template<typename T>
using callback_t = std::function<void(resolve_cb_t<T>, reject_cb_t)>;

template <typename T>
class Promise
{
  struct PromiseData {
    T value;
    int id;
    PromiseStates state = PromiseStates::Pending;
    std::vector<resolve_cb_t<T>> thenHandlers;
    std::vector<reject_cb_t> catchHandlers;

    void _resolve(T);
    void _reject(const std::exception_ptr&);
  };

  std::shared_ptr<PromiseData> data;
public:
  inline Promise(callback_t<T>);
  inline ~Promise();

  template <typename U>
  Promise<U> then(std::function<U(T)>);

};

template<typename T>
inline Promise<T>::Promise(callback_t<T> callback)
{
  data = std::make_shared<PromiseData>();
  data->id = promise_id++;
  try {
    callback([data{this->data}](auto v){
      PromiseEngine::enqueue([data, v]() {
        //_resolve(std::forward<decltype(v)>(v));
        data->_resolve(v);
      });
    }, [data{this->data}](auto e){
      PromiseEngine::enqueue([data, e]() {
        //_reject(std::forward<decltype(e)>(e));
        data->_reject(e);
      });
    });
  } catch (...) {
    data->_reject(std::current_exception());
  }
}

template<typename T>
inline Promise<T>::~Promise()
{
  std::cout << "destructing " << data->id << std::endl;
}

template <typename T>
template <typename U>
Promise<U> Promise<T>::then(std::function<U(T)> cb)
{
  return Promise<U>([data{this->data}, cb](auto resolve, auto reject) {
      if (data->state == PromiseStates::Fulfilled) {
        try {
          resolve(cb(data->value));
        } catch (...) {
          reject(std::current_exception());
        }
      } else if (data->state == PromiseStates::Pending) {
        data->thenHandlers.push_back([data, resolve, reject, cb](T v){
          try {
            resolve(cb(data->value));
          } catch (...) {
            reject(std::current_exception());
          }
        });
      }
  });
}

template<typename T>
void Promise<T>::PromiseData::_resolve(T val)
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
void Promise<T>::PromiseData::_reject(const std::exception_ptr& e)
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
} // end namespace Promise
