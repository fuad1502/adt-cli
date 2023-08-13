#pragma once
#include "cli-list.hpp"
#include <memory>
#include <sstream>

template<typename T>
class Tree {
public:
  template<typename Y> friend class BST;
private:
  Tree(const T& data) : data(data), left(nullptr), right(nullptr) {};
  T data;
  std::shared_ptr<Tree> left;
  std::shared_ptr<Tree> right;
};

template<typename T>
class BST {
public:
  BST() : head(nullptr), n(0) {};
  std::shared_ptr<Tree<T>> search(const T& data);
  void insert(const T& data);
  void remove(std::shared_ptr<Tree<T>> tree);
  std::string traverse();
  BST<T> balance();
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const BST<U>& bst);
private:
  std::shared_ptr<Tree<T>> head;
  int n;
  static std::string treeString(std::shared_ptr<Tree<T>> tree, int& nSlashesLeftParent, int& nSlashesRightParent);
  static std::shared_ptr<Tree<T>> searchInTree(const T& data, std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& treeParent);
  static std::shared_ptr<Tree<T>> findMinimumInTree(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& minParent);
  static std::shared_ptr<Tree<T>> findMaximumInTree(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& maxParent);
  static std::string traverseInTree(std::shared_ptr<Tree<T>> tree);
  void moveTreeChildUp(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>> treeChild, std::shared_ptr<Tree<T>> treeParent);
  static void createBalanceTree(std::shared_ptr<Tree<T>> tree, int n);
  static void createList(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Node<std::shared_ptr<Tree<T>>>>& head, std::shared_ptr<Node<std::shared_ptr<Tree<T>>>>& tail);
};

template<typename T>
void BST<T>::insert(const T& data) {
  if(head == nullptr) {
    head = std::shared_ptr<Tree<T>>(new Tree(data));
    n = 1;
    return;
  } 
  std::shared_ptr<Tree<T>> tree = head;
  while(true){
    if(data > tree->data) {
      if(tree->right == nullptr) {
        tree->right = std::shared_ptr<Tree<T>>(new Tree(data));
        n += 1;
      }
      tree = tree->right;
    } else if(data < tree->data) {
      if(tree->left == nullptr) {
        tree->left = std::shared_ptr<Tree<T>>(new Tree(data));
        n += 1;
      }
      tree = tree->left;
    } else {
      break;
    }
  }
  return;
}

template<typename T>
std::shared_ptr<Tree<T>> BST<T>::search(const T& data) {
  std::shared_ptr<Tree<T>> unused;
  return searchInTree(data, head, unused);
}

template<typename T>
void BST<T>::remove(std::shared_ptr<Tree<T>> tree)
{
  if(tree == nullptr) {
    // trying to delete a null tree
    return;
  }
  // find parent of node
  std::shared_ptr<Tree<T>> treeParent;
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
    std::shared_ptr<Tree<T>> maxParent;
    std::shared_ptr<Tree<T>> max;
    // find the minimum
    max = findMaximumInTree(tree->left, maxParent);
    // delete reference to minimum
    if(maxParent == nullptr) {
      tree->right = nullptr;
    } else {
      maxParent->right = nullptr;
    }
    // move minimum data
    tree->data = max->data;
  }
  n -= 1;
}

template<typename T>
std::string BST<T>::traverse()
{
  return traverseInTree(head);
}

template<typename T>
using pTree = std::shared_ptr<Tree<T>>;
template<typename T>
using pNode = std::shared_ptr<Node<T>>;

template<typename T>
void BST<T>::createBalanceTree(pTree<T> tree, int n)
{
  if(n == 0) {
    ;
  } else if(n == 1) {
    tree->left = std::shared_ptr<Tree<T>>(new Tree(0));
  } else {
    tree->left = std::shared_ptr<Tree<T>>(new Tree(0));
    tree->right = std::shared_ptr<Tree<T>>(new Tree(0));
    // create half (round up) the remaining balanced trees to the left
    createBalanceTree(tree->left, (n - 2 + 1) / 2);
    // create half (round down) the remaining balanced trees to the right
    createBalanceTree(tree->right, (n - 2) / 2);
  }
}

template<typename T>
void BST<T>::createList(pTree<T> tree, pNode<pTree<T>>& head, pNode<pTree<T>>& tail)
{
  if(tree == nullptr) {
    head = nullptr;
    tail = nullptr;
  } else {
    // recurse left
    pNode<pTree<T>> leftHead, leftTail;
    createList(tree->left, leftHead, leftTail);
    // create node
    pNode<pTree<T>> node = std::shared_ptr<Node<pTree<T>>>(new Node<pTree<T>>(tree));
    // recurse right
    pNode<pTree<T>> rightHead, rightTail;
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

#include <iostream>
template<typename T>
BST<T> BST<T>::balance()
{
  // manually create balanced tree of length n
  BST<T> balanced;
  balanced.insert(0);
  createBalanceTree(balanced.head, n - 1);
  // create tree list of the balanced tree
  pNode<pTree<T>> balancedListHead, balancedListTail;
  createList(balanced.head, balancedListHead, balancedListTail);
  // create tree list of the original tree
  pNode<pTree<T>> listHead, listTail;
  createList(head, listHead, listTail);
  // copy data from original to balanced tree
  pNode<pTree<T>> node = listHead;
  pNode<pTree<T>> balancedNode = balancedListHead;
  while(node != nullptr) {
    balancedNode->data->data = node->data->data;
    node = node->next;
    balancedNode = balancedNode->next;
  }
  // return balanced tree
  return balanced;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const BST<T>& bst)
{ 
  int unused1, unused2;
  os << BST<T>::treeString(bst.head, unused1, unused2);
  return os;
}

template<typename T>
std::shared_ptr<Tree<T>> BST<T>::searchInTree(const T& data, std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& treeParent)
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
std::shared_ptr<Tree<T>> BST<T>::findMinimumInTree(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& minParent)
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
std::shared_ptr<Tree<T>> BST<T>::findMaximumInTree(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& maxParent)
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
std::string BST<T>::traverseInTree(std::shared_ptr<Tree<T>> tree)
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
void BST<T>::moveTreeChildUp(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>> treeChild, std::shared_ptr<Tree<T>> treeParent) {
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
std::string BST<T>::treeString(std::shared_ptr<Tree<T>> tree, int& nSlashesLeftParent, int& nSlashesRightParent)
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
