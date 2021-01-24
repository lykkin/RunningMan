#pragma once

#include "./promise_engine.h"

#include <functional>
#include <memory>
#include <exception>
#include <vector>

namespace RunningMan
{

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
public:
  inline Promise(callback_t<T>);
  inline ~Promise();

  template <typename U>
  Promise<U> then(std::function<U(T)>);

private:
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
};
} // end namespace Promise
