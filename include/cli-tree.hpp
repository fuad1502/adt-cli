#pragma once
#include "cli-list.hpp"
#include <memory>
#include <sstream>

template<typename T>
struct Tree {
  T data;
  std::shared_ptr<Tree> left;
  std::shared_ptr<Tree> right;
};

template<typename T>
using pTree = std::shared_ptr<Tree<T>>;

template<typename T>
class DictTree {
public:
  DictTree() : head(nullptr), n(0) {};
  pTree<T> search(const T& data);
  void insert(const T& data);
  void remove(pTree<T> tree);
  std::string traverse();
  DictTree<T> balance();
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const DictTree<U>& bst);
private:
  pTree<T> head;
  int n;
  static std::string treeString(pTree<T> tree, int& nSlashesLeftParent, int& nSlashesRightParent);
  static pTree<T> searchInTree(const T& data, pTree<T> tree, pTree<T>& treeParent);
  static pTree<T> findMinimumInTree(pTree<T> tree, pTree<T>& minParent);
  static pTree<T> findMaximumInTree(pTree<T> tree, pTree<T>& maxParent);
  static std::string traverseInTree(pTree<T> tree);
  void moveTreeChildUp(pTree<T> tree, pTree<T> treeChild, pTree<T> treeParent);
  static void createBalanceTree(pTree<T> tree, int n);
  static void createList(pTree<T> tree, pList<pTree<T>>& head, pList<pTree<T>>& tail);
};

template<typename T>
void DictTree<T>::insert(const T& data) {
  if(head == nullptr) {
    head = std::shared_ptr<Tree<T>>(new Tree<T>);
    head->data = data;
    n = 1;
    return;
  } 
  pTree<T> tree = head;
  while(true){
    if(data > tree->data) {
      if(tree->right == nullptr) {
        tree->right = std::shared_ptr<Tree<T>>(new Tree<T>);
        tree->right->data = data;
        n += 1;
      }
      tree = tree->right;
    } else if(data < tree->data) {
      if(tree->left == nullptr) {
        tree->left = std::shared_ptr<Tree<T>>(new Tree<T>);
        tree->left->data = data;
        n += 1;
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
  n -= 1;
}

template<typename T>
std::string DictTree<T>::traverse()
{
  return traverseInTree(head);
}

template<typename T>
void DictTree<T>::createBalanceTree(pTree<T> tree, int n)
{
  if(n == 0) {
    ;
  } else if(n == 1) {
    tree->left = std::shared_ptr<Tree<T>>(new Tree<T>);
  } else {
    tree->left = std::shared_ptr<Tree<T>>(new Tree<T>);
    tree->right = std::shared_ptr<Tree<T>>(new Tree<T>);
    // create half (round up) of the remaining balanced trees to the left
    createBalanceTree(tree->left, (n - 2 + 1) / 2);
    // create half (round down) of the remaining balanced trees to the right
    createBalanceTree(tree->right, (n - 2) / 2);
  }
}

template<typename T>
void DictTree<T>::createList(pTree<T> tree, pList<pTree<T>>& head, pList<pTree<T>>& tail)
{
  if(tree == nullptr) {
    head = nullptr;
    tail = nullptr;
  } else {
    // recurse left
    pList<pTree<T>> leftHead, leftTail;
    createList(tree->left, leftHead, leftTail);
    // create node
    pList<pTree<T>> node = std::shared_ptr<List<pTree<T>>>(new List<pTree<T>>(tree));
    // recurse right
    pList<pTree<T>> rightHead, rightTail;
    createList(tree->right, rightHead, rightTail);
    // Connect node with left recursion
    if(leftTail != nullptr) {
      leftTail->next = node;
      node->previous = leftTail;
      head = leftHead;
    } else {
      head = node;
    }
    // Connect node with right recursion
    if(rightHead != nullptr) {
      node->next = rightHead;
      rightHead->previous = node;
      tail = rightTail;
    } else {
      tail = node;
    }
  }
}

template<typename T>
DictTree<T> DictTree<T>::balance()
{
  // manually create balanced tree of length n
  DictTree<T> balanced;
  balanced.insert(0);
  createBalanceTree(balanced.head, n - 1);
  // create tree list of the balanced tree
  pList<pTree<T>> balancedListHead, balancedListTail;
  createList(balanced.head, balancedListHead, balancedListTail);
  // create tree list of the original tree
  pList<pTree<T>> listHead, listTail;
  createList(head, listHead, listTail);
  // copy data from original to balanced tree
  pList<pTree<T>> node = listHead;
  pList<pTree<T>> balancedList = balancedListHead;
  while(node != nullptr) {
    balancedList->data->data = node->data->data;
    node = node->next;
    balancedList = balancedList->next;
  }
  // return balanced tree
  return balanced;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const DictTree<T>& bst)
{ 
  int unused1, unused2;
  os << DictTree<T>::treeString(bst.head, unused1, unused2);
  return os;
}

template<typename T>
pTree<T> DictTree<T>::searchInTree(const T& data, pTree<T> tree, pTree<T>& treeParent)
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
pTree<T> DictTree<T>::findMinimumInTree(pTree<T> tree, pTree<T>& minParent)
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
pTree<T> DictTree<T>::findMaximumInTree(pTree<T> tree, pTree<T>& maxParent)
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
std::string DictTree<T>::traverseInTree(pTree<T> tree)
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
std::string DictTree<T>::treeString(pTree<T> tree, int& nSlashesLeftParent, int& nSlashesRightParent)
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
