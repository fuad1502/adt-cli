#pragma once
#include <memory>

template<typename T>
struct List {
  T data;
  std::shared_ptr<List> next;
  std::shared_ptr<List> previous;
};

template<typename T>
using pList = std::shared_ptr<List<T>>;

template<typename T>
class DictList {
public:
  std::shared_ptr<List<T>> search(const T& data);
  void insert(const T& data);
  void remove(std::shared_ptr<List<T>> node);
private:
  std::shared_ptr<List<T>> head;
};
