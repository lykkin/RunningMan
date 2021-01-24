#include <iostream>
#include "./src/promise.cpp"
int main() {
  RunningMan::Promise<int>([](auto resolve, auto reject) {
      std::cout << "cons" << std::endl;
      resolve(2);
  }).then<bool>([](int v) {
    std::cout << "first then " << v << std::endl;
    return true;
  }).then<int>([](bool b) {
    std::cout << "second then " << b << std::endl;
    return 3;
  });
  std::cout << "gap" << std::endl;
  while (!RunningMan::PromiseEngine::isDone()) {
    RunningMan::PromiseEngine::run();
  }
  return 0;
}
