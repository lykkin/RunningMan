load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_testonly=True)
cc_test(
  name = "test_promise",
  size = "small",
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//:running-man",
    "@com_google_googletest//:gtest_main",
  ],
)
