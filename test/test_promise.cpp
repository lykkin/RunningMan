#include "gtest/gtest.h"
#include "running-man/promise.h"
#include "running-man/promise_engine.h"

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

TEST(PromiseTest, VoidTest) {
  int calls = 0;
  RunningMan::Promise<void>([&](auto resolve, auto reject) {
      calls++;
      resolve();
  }).then<void>([&]() {
      calls++;
  });
  while (!RunningMan::PromiseEngine::isDone()) {
    RunningMan::PromiseEngine::run();
  }
  EXPECT_EQ(calls, 2);
}

TEST(PromiseTest, VoidToValueTest) {
  int calls = 0;
  RunningMan::Promise<void>([&](auto resolve, auto reject) {
      calls++;
      resolve();
  }).then<int>([&]() {
      calls++;
      return 1;
  }).then<void>([&](auto v) {
      calls++;
      EXPECT_EQ(v, 1);
  });
  while (!RunningMan::PromiseEngine::isDone()) {
    RunningMan::PromiseEngine::run();
  }
  EXPECT_EQ(calls, 3);
}

TEST(PromiseTest, ValueToVoidTest) {
  int calls = 0;
  RunningMan::Promise<int>([&](auto resolve, auto reject) {
      calls++;
      resolve(1);
  }).then<void>([&](auto v) {
      EXPECT_EQ(v, 1);
      calls++;
  }).then<int>([&]() {
      calls++;
      return 0;
  });
  while (!RunningMan::PromiseEngine::isDone()) {
    RunningMan::PromiseEngine::run();
  }
  EXPECT_EQ(calls, 3);
}

// TODO: get this working
//TEST(PromiseTest, DefaultTest) {
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
