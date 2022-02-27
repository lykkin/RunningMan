#include "gtest/gtest.h"
#include "running-man/promise.h"
#include "running-man/promise_engine.h"

// Demonstrate some basic assertions.
TEST(PromiseTest, ResolveTest) {
  int result = 0;
  RunningMan::Promise<int>([](auto resolve, auto reject) {
    resolve(1);
  }).then<int>([&](auto r) {
    result = r;
    return 0;
  });
  while (!RunningMan::PromiseEngine::isDone()) {
    RunningMan::PromiseEngine::run();
  }
  EXPECT_EQ(result, 1);
}
