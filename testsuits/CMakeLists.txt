add_executable(cli-tree-testsuit cli-tree-testsuit.cpp)
target_include_directories(cli-tree-testsuit PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cli-tree-testsuit PRIVATE gtest_main)
