#pragma once

#include <functional>
#include <memory>
#include <exception>
#include <vector>

#include "running-man/promise_types.h"
#include "running-man/promise_engine.h"

namespace RunningMan
{
static int promise_id = 0;

enum class PromiseStates {
  Pending,
  Fulfilled,
  Rejected
};

template <typename T = void>
class Promise
{
  struct PromiseData {
    T value;
    int id;
    PromiseStates state = PromiseStates::Pending;
    std::vector<resolve_cb_t<T>> thenHandlers;
    std::vector<reject_cb_t> catchHandlers;

    void _resolve(T val)
    {
      if (state > PromiseStates::Pending) {
        return;
      }
      value = val;
      state = PromiseStates::Fulfilled;
      for (auto h : thenHandlers) {
          h(val);
      }
    };
    void _reject(const std::exception_ptr&);
  };

  std::shared_ptr<PromiseData> data;
public:
  template <typename U = void>
  Promise<U> then(std::function<U(T)>);
  inline Promise<T>(callback_t<T>);
  inline ~Promise<T>();

};

template <typename T, typename U>
struct thenExec {
  static void exec(std::function<U(T)> f, T v, resolve_cb_t<U> resolve) {
    resolve(f(v));
  };
};

template <typename T>
struct thenExec<T, void> {
  static void exec(std::function<void(T)> f, T v, void_resolve_cb_t resolve) {
    f(v);
    resolve();
  };
};

template <typename T>
template <typename U>
Promise<U> Promise<T>::then(std::function<U(T)> cb)
{
  return Promise<U>([data{this->data}, cb](auto resolve, auto reject) {
      if (data->state == PromiseStates::Fulfilled) {
        try {
          thenExec<T, U>::exec(cb, data->value, resolve);
          //resolve(cb(data->value));
        } catch (...) {
          reject(std::current_exception());
        }
      } else if (data->state == PromiseStates::Pending) {
        data->thenHandlers.push_back([data, resolve, reject, cb](T v){
          try {
            thenExec<T, U>::exec(cb, data->value, resolve);
            //resolve(cb(data->value));
          } catch (...) {
            reject(std::current_exception());
          }
        });
      }
  });
};

// Implementations

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
