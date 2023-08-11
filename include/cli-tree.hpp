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
  void remove(std::shared_ptr<Tree<T>> p);
  std::string treeString(std::shared_ptr<Tree<T>> p);
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const BST<U>& bst);
private:
  std::shared_ptr<Tree<T>> head;
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

int columns(const std::string& s);
std::string concatenateLines(const std::string& data, const std::string& left, const std::string& right);

template<typename T>
std::string BST<T>::treeString(std::shared_ptr<Tree<T>> tree)
{
  if(tree == nullptr) {
    return "";
  } else {
    std::string dataString = std::to_string(tree->data);
    if(dataString.length() == 1) {
      dataString = ' ' + dataString + ' ';
    }
    // Left branch
    std::string leftString = treeString(tree->left);
    int leftStringColumns = columns(leftString);
    // Right branch
    std::string rightString = treeString(tree->right);
    int rightStringColumns = columns(rightString);
    // Calculate length of tree trunk
    int childColumn = std::max(leftStringColumns, rightStringColumns);
    int nSlashes = childColumn / 2;
    std::string s;
    // Print tree root node
    s += std::string(childColumn, ' ');
    s += dataString;
    s += std::string(childColumn, ' ');
    s += '\n';
    // Print tree trunk
    for(int i = 0; i < nSlashes; i++) {
      s += std::string(childColumn - i - 1, ' ');
      if(leftString.length() != 0)
        s += '/';
      else 
        s += ' ';
      s += std::string(i, ' ');
      s += std::string(dataString.length(), ' ');
      s += std::string(i, ' ');
      if(rightString.length() != 0)
        s += '\\';
      else 
        s += ' ';
      s += std::string(childColumn - i - 1, ' ');
      s += '\n';
    }
    // Print concatenation of left and right tree
    s += concatenateLines(dataString, leftString, rightString);
    return s;
  }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const BST<T>& bst)
{ 
  os << bst.treeString(bst.head);
  return os;
}
