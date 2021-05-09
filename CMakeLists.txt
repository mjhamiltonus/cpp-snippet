cmake_minimum_required(VERSION 3.19)
project(TestCode)

set(CMAKE_CXX_STANDARD 14)

add_executable(useAfterMove useAfterMove.cpp)
target_compile_options(useAfterMove PRIVATE -Wall -Wextra -pedantic -Werror)


add_executable(privateAccess privateAccess.cpp)
target_compile_options(privateAccess PRIVATE -Wall -Wextra -pedantic -Werror)
