#pragma once
#include <memory>

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
