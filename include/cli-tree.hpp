#pragma once
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
  std::shared_ptr<Tree<T>> search(const T& data);
  void insert(const T& data);
  void remove(std::shared_ptr<Tree<T>> tree);
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const BST<U>& bst);
private:
  std::shared_ptr<Tree<T>> head;
  static std::string treeString(std::shared_ptr<Tree<T>> tree, int& nSlashesLeftParent, int& nSlashesRightParent);
};

template<typename T>
void BST<T>::insert(const T& data) {
  if(head == nullptr) {
    head = std::shared_ptr<Tree<T>>(new Tree(data));
    return;
  } 

  std::shared_ptr<Tree<T>> tree = head;
  while(true){
    if(data > tree->data) {
      if(tree->right == nullptr) {
        tree->right = std::shared_ptr<Tree<T>>(new Tree(data));
      }
      tree = tree->right;
    } else if(data < tree->data) {
      if(tree->left == nullptr) {
        tree->left = std::shared_ptr<Tree<T>>(new Tree(data));
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
  std::shared_ptr<Tree<T>> tree = head;
  while(head != nullptr) {
    if(data > tree->data) {
      tree = tree->right;
    } else if(data < tree->data) {
      tree = tree->left;
    } else {
      break;
    }
  }
  return tree;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const BST<T>& bst)
{ 
  int unused1, unused2;
  os << BST<T>::treeString(bst.head, unused1, unused2);
  return os;
}

int columns(const std::string& s);

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
    if(leftString.length() == 2) {
      // Special case for single character leaf node
      leftString = std::string(1, leftString[0]) + " \n";
      nSlashesLeft = 1;
    }
    int leftColumns = columns(leftString);
    // Right tree
    int nSlashesRight;
    std::string rightString = treeString(tree->right, unused, nSlashesRight);
    if(rightString.length() == 2) {
      // Special case for single character leaf node
      rightString = " " + std::string(1, rightString[0]) + "\n";
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
