add_library(verimatlang SHARED
    src/parser.cpp
    src/interpreter.cpp
    src/io.cpp
)
target_include_directories(verimatlang
    PRIVATE src
    PUBLIC include)
target_compile_features(verimatlang PRIVATE cxx_std_17)

add_executable(verimatlang_test
    tests/parser_test.cpp
    tests/interpreter_test.cpp
)
target_link_libraries(verimatlang_test
    GTest::gtest_main
)
target_include_directories(verimatlang_test
    PRIVATE src include)
target_compile_features(verimatlang_test PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(verimatlang_test)
