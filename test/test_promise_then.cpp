#include "running-man/promise.h"
#include "running-man/promise_engine.h"
#include "gtest/gtest.h"

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

// TEST(PromiseTest, VoidTest) {
//   int calls = 0;
//   RunningMan::Promise<void>([&](auto resolve, auto reject) {
//       calls++;
//       resolve();
//   }).then<void>([&]() {
//       calls++;
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//       RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 2);
// }
//
// TEST(PromiseTest, VoidToValueTest) {
//   int calls = 0;
//   RunningMan::Promise<void>([&](auto resolve, auto reject) {
//       calls++;
//       resolve();
//   }).then<int>([&]() {
//       calls++;
//       return 1;
//   }).then<void>([&](auto v) {
//       calls++;
//       EXPECT_EQ(v, 1);
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//       RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 3);
// }
//
// TEST(PromiseTest, ValueToVoidTest) {
//   int calls = 0;
//   RunningMan::Promise<int>([&](auto resolve, auto reject) {
//       calls++;
//       resolve(1);
//   }).then<void>([&](auto v) {
//       EXPECT_EQ(v, 1);
//       calls++;
//   }).then<int>([&]() {
//       calls++;
//       return 0;
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//       RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 3);
// }
//
// TEST(PromiseTest, ForkTest) {
//   int calls = 0;
//   auto p = RunningMan::Promise<int>([&](auto resolve, auto reject) {
//       calls++;
//       resolve(1);
//   });
//   p.then<void>([&](auto v) {
//       EXPECT_EQ(v, 1);
//       calls++;
//   });
//   p.then<int>([&](auto v) {
//       calls++;
//       EXPECT_EQ(v, 1);
//       return v;
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//       RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 3);
// }
//
// TEST(PromiseTest, ThenAfterExecTest) {
//   int calls = 0;
//   auto p = RunningMan::Promise<int>([&](auto resolve, auto reject) {
//       calls++;
//       resolve(1);
//   }).then<void>([&](auto v) {
//       EXPECT_EQ(v, 1);
//       calls++;
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//       RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 2);
//   p.then<int>([&](){
//       calls++;
//       return 0;
//   });
//   EXPECT_EQ(calls, 3);
// }
//
// TEST(PromiseTest, NestedPromises) {
//   int calls = 0;
//   auto p = RunningMan::Promise<int>([&](auto resolve, auto reject) {
//       calls++;
//       resolve(1);
//   }).then<RunningMan::Promise<int>>([&](auto v) {
//       EXPECT_EQ(calls, 1);
//       EXPECT_EQ(v, 1);
//       calls++;
//       return RunningMan::Promise<void>([&](auto resolve, auto reject) {
//         EXPECT_EQ(calls, 2);
//         calls++;
//       }).then<int>([&]() {
//         EXPECT_EQ(calls, 3);
//         calls++;
//         return 2;
//       });
//   }).then<bool>([&](auto v) {
//     EXPECT_EQ(calls, 4);
//     EXPECT_EQ(v, 2);
//     calls++;
//     return true;
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//       RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 5);
// }

// TODO: get this working
// TEST(PromiseTest, DefaultTest) {
//  int calls = 0;
//  RunningMan::Promise([&](auto resolve, auto reject) -> void {
//      calls++;
//      resolve();
//  }).then([&]() {
//      calls++;
//  }).then<int>([&](){
//      calls++;
//      return 0;
//  }).then([&](auto v) {
//      EXPECT_EQ(v, 0);
//      calls++;
//  });
//  RunningMan::Promise<int>([&](auto resolve, auto reject) -> void {
//      calls++;
//      resolve();
//      return 1;
//  }).then([&]() {
//      calls++;
//      EXPECT_EQ(v, 1);
//  }).then<int>([&](){
//      calls++;
//      return 0;
//  }).then([&](auto v) {
//      calls++;
//      EXPECT_EQ(v, 0);
//  });
//  while (!RunningMan::PromiseEngine::isDone()) {
//    RunningMan::PromiseEngine::run();
//  }
//  EXPECT_EQ(calls, 8);
//}
