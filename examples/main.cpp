#include "cli-tree.hpp"

int main (int argc, char *argv[]) {
  BST<int> bst;
  std::shared_ptr<Tree<int>> tree = bst.search(1);
  return 0;
}
