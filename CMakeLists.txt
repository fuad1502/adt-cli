cmake_minimum_required(VERSION 3.14)

project(cli-tree)

# generate compile-commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cli-tree library
add_library(cli-tree STATIC)
target_include_directories(cli-tree PRIVATE include)
target_sources(cli-tree PRIVATE src/cli-tree.cpp)

# Examples
add_subdirectory(examples)

if(CLI_ADT_ENABLE_TESTING)
  add_subdirectory(lib/googletest)
  enable_testing()
  add_subdirectory(testsuits)
  add_test(NAME cli-tree-testsuit COMMAND cli-tree-testsuit)
endif()
