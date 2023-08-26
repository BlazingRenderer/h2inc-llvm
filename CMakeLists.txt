cmake_minimum_required(VERSION 3.23)
project(h2inc-llvm LANGUAGES C CXX)

find_package(Clang REQUIRED)

add_executable(h2inc-llvm ${PROJECT_SOURCE_DIR}/source/h2inc-llvm.cpp)
target_link_libraries(h2inc-llvm PRIVATE libclang)
