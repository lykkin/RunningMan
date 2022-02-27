load("@rules_cc//cc:defs.bzl", "cc_library")
cc_library(
    name="running-man",
    hdrs=glob(["include/**/*.h"]),
    srcs=glob(["src/*.cpp"]),
    includes=["include"],
    visibility = ["//visibility:public"],
)
