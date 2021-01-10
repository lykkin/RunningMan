#include <iostream>
#include "./src/promise.cpp"
int main() {
  RunningMan::Promise<int>([](auto resolve, auto reject) {
      resolve(1);
  }).then<int>([](int v) {
    std::cout << v << std::endl;
    return v;
  });
  return 0;
}
