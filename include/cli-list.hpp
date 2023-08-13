#pragma once
#include <memory>

template<typename T>
class Node {
public:
  template<typename Y>
  friend class List;
  template<typename Y>
  friend class BST;
private:
  Node(const T& data) : data(data), next(nullptr), previous(nullptr) {};
  T data;
  std::shared_ptr<Node> next;
  std::shared_ptr<Node> previous;
};

template<typename T>
class List {
public:
  std::shared_ptr<Node<T>> search(const T& data);
  void insert(const T& data);
  void remove(std::shared_ptr<Node<T>> node);
private:
  std::shared_ptr<Node<T>> head;
};
