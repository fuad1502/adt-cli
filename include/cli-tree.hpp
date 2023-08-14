#pragma once
#include "cli-list.hpp"
#include <memory>
#include <sstream>

// Tree data structure
template<typename T>
struct Tree {
  T data;
  std::shared_ptr<Tree> left;
  std::shared_ptr<Tree> right;
};

template<typename T>
using pTree = std::shared_ptr<Tree<T>>;

// Functions on Tree DS
template<typename T>
int getNumberOfNodes(pTree<T> tree);
template<typename T>
std::string treeString(pTree<T> tree, int& nSlashesLeftParent, int& nSlashesRightParent);
template<typename T>
pTree<T> searchInTree(const T& data, pTree<T> tree, pTree<T>& treeParent);
template<typename T>
pTree<T> findMinimumInTree(pTree<T> tree, pTree<T>& minParent);
template<typename T>
pTree<T> findMaximumInTree(pTree<T> tree, pTree<T>& maxParent);
template<typename T>
std::string traverseInTree(pTree<T> tree);

template<typename T>
class DictTree {
public:
  DictTree() : head(nullptr) {};
  DictTree(pTree<T> tree);
  pTree<T> search(const T& data);
  void insert(const T& data);
  void remove(pTree<T> tree);
  std::string traverse();
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const DictTree<U>& bst);
  pTree<T> data();
private:
  pTree<T> head;
  void moveTreeChildUp(pTree<T> tree, pTree<T> treeChild, pTree<T> treeParent);
};

template<typename T>
void DictTree<T>::insert(const T& data) {
  if(head == nullptr) {
    head = std::shared_ptr<Tree<T>>(new Tree<T>);
    head->data = data;
    return;
  } 
  pTree<T> tree = head;
  while(true){
    if(data > tree->data) {
      if(tree->right == nullptr) {
        tree->right = std::shared_ptr<Tree<T>>(new Tree<T>);
        tree->right->data = data;
      }
      tree = tree->right;
    } else if(data < tree->data) {
      if(tree->left == nullptr) {
        tree->left = std::shared_ptr<Tree<T>>(new Tree<T>);
        tree->left->data = data;
      }
      tree = tree->left;
    } else {
      break;
    }
  }
}

template<typename T>
pTree<T> DictTree<T>::search(const T& data) {
  pTree<T> unused;
  return searchInTree(data, head, unused);
}

template<typename T>
void DictTree<T>::remove(pTree<T> tree)
{
  if(tree == nullptr) {
    // trying to delete a null tree
    return;
  }
  // find parent of node
  pTree<T> treeParent;
  if(searchInTree(tree->data, head, treeParent) != tree)
  {
    // tree is not in the head tree
    return;
  }
  if(tree->left == nullptr && tree->right == nullptr) {
    moveTreeChildUp(tree, nullptr, treeParent);
  } else if(tree->left == nullptr && tree->right != nullptr) {
    moveTreeChildUp(tree, tree->right, treeParent);
  } else if(tree->left != nullptr && tree->right == nullptr) {
    moveTreeChildUp(tree, tree->left, treeParent);
  } else {
    pTree<T> maxParent;
    pTree<T> max;
    // find the maximum
    max = findMaximumInTree(tree->left, maxParent);
    // delete reference to maximum
    if(maxParent == nullptr) {
      tree->right = nullptr;
    } else {
      maxParent->right = nullptr;
    }
    // move maximum data
    tree->data = max->data;
  }
}

template<typename T>
std::string DictTree<T>::traverse()
{
  return traverseInTree(head);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const DictTree<T>& bst)
{ 
  int unused1, unused2;
  os << treeString(bst.head, unused1, unused2);
  return os;
}

template<typename T>
pTree<T> DictTree<T>::data()
{
  return head;
}

template<typename T>
int getNumberOfNodes(pTree<T> tree)
{
  if(tree == nullptr) {
    return 0;
  } else {
    return 1 + getNumberOfNodes(tree->left) + getNumberOfNodes(tree->right);
  }
}

template<typename T>
pTree<T> searchInTree(const T& data, pTree<T> tree, pTree<T>& treeParent)
{
  treeParent = nullptr;
  while(tree != nullptr) {
    if(data > tree->data) {
      treeParent = tree;
      tree = tree->right;
    } else if(data < tree->data) {
      treeParent = tree;
      tree = tree->left;
    } else {
      break;
    }
  }
  return tree;
}

template<typename T>
pTree<T> findMinimumInTree(pTree<T> tree, pTree<T>& minParent)
{
  minParent = nullptr;
  if(tree == nullptr) {
    return nullptr;
  }
  while(tree->left != nullptr) {
    minParent = tree;
    tree = tree->left;
  }
  return tree;
}

template<typename T>
pTree<T> findMaximumInTree(pTree<T> tree, pTree<T>& maxParent)
{
  maxParent = nullptr;
  if(tree == nullptr) {
    return nullptr;
  }
  while(tree->left != nullptr) {
    maxParent = tree;
    tree = tree->right;
  }
  return tree;
}

template<typename T>
std::string traverseInTree(pTree<T> tree)
{
  if(tree == nullptr) {
    return "";
  } else {
    std::string s;
    s = traverseInTree(tree->left);
    s += std::to_string(tree->data) + " ";
    s += traverseInTree(tree->right);
    return s;
  }
}

template<typename T>
void DictTree<T>::moveTreeChildUp(pTree<T> tree, pTree<T> treeChild, pTree<T> treeParent) {
    if(treeParent == nullptr) {
      // tree is also the head
      head = treeChild;
    } else {
      // tree is just a leaf node
      if(treeParent->left == tree) {
        treeParent->left = treeChild;
      } else {
        treeParent->right = treeChild;
      }
    }
}

int columns(const std::string& s);
std::string addColumn(const std::string& leftColumn, const std::string& s, const std::string& rightColumn);

template<typename T>
std::string treeString(pTree<T> tree, int& nSlashesLeftParent, int& nSlashesRightParent)
{
  if(tree == nullptr) {
    nSlashesLeftParent = 0;
    nSlashesRightParent = 0;
    return "";
  } else {
    // Root node
    std::string dataString = std::to_string(tree->data);
    // Left tree
    int unused;
    int nSlashesLeft;
    std::string leftString = treeString(tree->left, nSlashesLeft, unused);
    if(nSlashesLeft == 0 && leftString.length() != 0) {
      // Special case for single character sub-tree
      leftString = addColumn("", leftString, " ");
      nSlashesLeft = 1;
    }
    int leftColumns = columns(leftString);
    // Right tree
    int nSlashesRight;
    std::string rightString = treeString(tree->right, unused, nSlashesRight);
    if(nSlashesRight == 0 && rightString.length() != 0) {
      // Special case for single character sub-tree
      rightString = addColumn(" ", rightString, "");
      nSlashesRight = 1;
    }
    int rightColumns = columns(rightString);
    // Print tree root node
    std::string s;
    s += std::string(leftColumns, ' ');
    s += dataString;
    s += std::string(rightColumns, ' ');
    s += '\n';
    // Print left and right tree
    std::string leftLine, rightLine;
    std::stringstream leftStream(leftString);
    std::stringstream rightStream(rightString);
    std::getline(leftStream, leftLine);
    std::getline(rightStream, rightLine);
    int i = 0; // number of printed lines
    while(leftLine.length() != 0 || rightLine.length() != 0) {
      if(i < nSlashesLeft) {
        // print connecting lines
        s += std::string(leftColumns - i - 1, ' ');
        s += '/';
        s += std::string(i, ' ');
      } else {
        // print left tree
        if(leftLine.length() != 0) {
          s += leftLine;
        } else {
          s += std::string(leftColumns, ' ');
        }
        std::getline(leftStream, leftLine);
      }
      // print root node gap
      s += std::string(dataString.length(), ' ');
      if(i < nSlashesRight) {
        // print connecting line
        s += std::string(i, ' ');
        s += '\\';
        s += std::string(rightColumns - i - 1, ' ');
      } else {
        // print right tree
        if(rightLine.length() != 0) {
          s += rightLine;
        } else {
          s += std::string(rightColumns, ' ');
        }
        std::getline(rightStream, rightLine);
      }
      // print new line
      s += '\n';
      i += 1;
    }
    // Report required number of slashes to parent
    nSlashesLeftParent = dataString.length() / 2 + rightColumns;
    nSlashesRightParent = dataString.length() / 2 + leftColumns;
    return s;
  }
}
