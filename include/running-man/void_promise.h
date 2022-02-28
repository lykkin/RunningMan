#pragma once

#include "running-man/promise_types.h"

namespace RunningMan
{

template <>
class Promise<void>
{
  struct PromiseData {
    int id;
    PromiseStates state = PromiseStates::Pending;
    std::vector<void_resolve_cb_t> thenHandlers;
    std::vector<reject_cb_t> catchHandlers;

    void _resolve()
    {
      if (state > PromiseStates::Pending) {
        return;
      }
      state = PromiseStates::Fulfilled;
      for (auto h : thenHandlers) {
          h();
      }
    };
    void _reject(const std::exception_ptr&);
  };

  std::shared_ptr<PromiseData> data;
public:
  inline Promise(void_callback_t);
  inline ~Promise();

  template <typename U = void>
  Promise<U> then(std::function<U()> cb);

};

template <typename U = void>
Promise<U> Promise<void>::then(std::function<U()> cb)
{
  return Promise<U>([data{this->data}, cb](auto resolve, auto reject) {
      if (data->state == PromiseStates::Fulfilled) {
        try {
          resolve(cb());
        } catch (...) {
          reject(std::current_exception());
        }
      } else if (data->state == PromiseStates::Pending) {
        data->thenHandlers.push_back([data, resolve, reject, cb](){
          try {
            resolve(cb());
          } catch (...) {
            reject(std::current_exception());
          }
        });
      }
  });
};

template <>
Promise<void> Promise<void>::then(std::function<void()> cb)
{
  return Promise<void>([data{this->data}, cb](auto resolve, auto reject) {
      if (data->state == PromiseStates::Fulfilled) {
        try {
          cb();
          resolve();
        } catch (...) {
          reject(std::current_exception());
        }
      } else if (data->state == PromiseStates::Pending) {
        data->thenHandlers.push_back([data, resolve, reject, cb](){
          try {
            cb();
            resolve();
          } catch (...) {
            reject(std::current_exception());
          }
        });
      }
  });
};


// Implementations

inline Promise<void>::Promise(void_callback_t callback)
{
  data = std::make_shared<PromiseData>();
  data->id = promise_id++;
  try {
    callback([data{this->data}](){
      PromiseEngine::enqueue([data]() {
        //_resolve(std::forward<decltype(v)>(v));
        data->_resolve();
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

inline Promise<void>::~Promise()
{
  std::cout << "destructing " << data->id << std::endl;
}

void Promise<void>::PromiseData::_reject(const std::exception_ptr& e)
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
