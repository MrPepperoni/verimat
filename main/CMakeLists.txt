include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG af442b4da0cd7a07b56fa709bd16571889dc7fda    # v3.0
)
FetchContent_MakeAvailable(argparse)
add_executable(verimat
    src/main.cpp
)
target_link_libraries(verimat PUBLIC
    verimatlang
    argparse
)
target_compile_features(verimat PRIVATE cxx_std_17)
