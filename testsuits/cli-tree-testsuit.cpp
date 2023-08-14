#include <gtest/gtest.h>
#include "cli-tree.hpp"

TEST(CliTreeTestsuit, All) {
  DictTree<int> tree; 
  EXPECT_EQ(tree.search(1), nullptr);
  tree.insert(1);
  EXPECT_NE(tree.search(1), nullptr);
  tree.insert(2);
  tree.insert(0);
  EXPECT_NE(tree.search(0), nullptr);
  EXPECT_NE(tree.search(2), nullptr);
}
