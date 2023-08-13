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
  std::string traverse();
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const BST<U>& bst);
private:
  std::shared_ptr<Tree<T>> head;
  static std::string treeString(std::shared_ptr<Tree<T>> tree, int& nSlashesLeftParent, int& nSlashesRightParent);
  static std::shared_ptr<Tree<T>> searchInTree(const T& data, std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& treeParent);
  static std::shared_ptr<Tree<T>> findMinimumInTree(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& minParent);
  static std::shared_ptr<Tree<T>> findMaximumInTree(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>>& maxParent);
  static std::string traverseInTree(std::shared_ptr<Tree<T>> tree);
  void moveTreeChildUp(std::shared_ptr<Tree<T>> tree, std::shared_ptr<Tree<T>> treeChild, std::shared_ptr<Tree<T>> treeParent);
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
}

template<typename T>
std::string BST<T>::traverse()
{
  return traverseInTree(head);
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
