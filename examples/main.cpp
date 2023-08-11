#include "cli-tree.hpp"
#include <iostream>

int main (int argc, char *argv[]) {
  BST<int> bst;
  bst.insert(23);
  bst.insert(12);
  bst.insert(34);
  bst.insert(56);
  bst.insert(6);
  bst.insert(64);
  bst.insert(50);
  std::cout << bst.treeString(bst.search(23)) << std::endl;
  return 0;
}
