//#include "gtest/gtest.h"
//#include "running-man/promise.h"
//#include "running-man/promise_engine.h"

// TEST(PromiseTest, CatchTest) {
//   int calls = 0;
//   RunningMan::Promise<void>([&](auto resolve, auto reject) {
//       reject(std::make_exception_ptr(std::runtime_error("oh no")));
//   }).then<void>([&]() {
//     FAIL();
//   }).catch<std::bad_exception, int>([&](auto e) {
//     calls++;
//     return 1;
//   }).catch<std::runtime_error, int>([&](auto e) {
//     calls++;
//     return 2;
//   }).then<void>([&](auto e) {
//     EXPECT_EQ(e, 2);
//   });
//   while (!RunningMan::PromiseEngine::isDone()) {
//     RunningMan::PromiseEngine::run();
//   }
//   EXPECT_EQ(calls, 1);
// }
