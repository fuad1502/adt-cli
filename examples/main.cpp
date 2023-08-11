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
  bst.insert(13);
  bst.insert(2);
  bst.insert(7);
  int tmp1, tmp2;
  std::cout << bst.treeString(bst.search(23), tmp1, tmp2) << std::endl;
  return 0;
}
