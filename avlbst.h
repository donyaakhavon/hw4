#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};


/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_==nullptr){
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
    }


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
 // findinsertion point and inserting a new node, to do!

 AVLNode<Key, Value>* current=static_cast<AVLNode<Key, Value>*>(this->root_);
 AVLNode<Key, Value>* parent = nullptr; 

 while(current!=nullptr) {
  parent = current;
  if(new_item.first == current->getKey()){
    // key already exists so update value
    current->setValue(new_item.second);
    return;
  }
  else if (new_item.first < current->getKey()) {
    current = current->getLeft();

  } else {
    current = current->getRight();
  }
 }

 // insert new node
 AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
 if(new_item.first < parent->getKey()) {
  parent->setLeft(newNode);
 } else {
  parent->setRight(newNode);
 }
 if(parent->getBalance() == 0) {
  // parent was balanced now unbalanced
  if(parent->getLeft() == newNode) {
    parent->setBalance(-1);
  } else {
    parent->setBalance(1);
  }
  insertFix(parent, newNode);
 }

}
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    // first, check if node is null
    if(node==nullptr) return;
    // case 1, if node has 2 chuldren, swap with predecessor and remove
    if(node->getLeft()!= nullptr && node->getRight()!=nullptr) {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
      nodeSwap(node, pred);

    }
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = nullptr;
    int8_t diff = 0;

    if(node->getLeft()!=nullptr) {
      child = node->getLeft();
    } else if(node->getRight()!= nullptr) {
      child = node->getRight();
    }
    // calculate diff for remove Fix
    if(parent!=nullptr) {
    if(parent->getLeft()==node){
      diff = 1;
     } else {
        diff = -1;
      }
    }
    
// update parent pointer
if(parent==nullptr) {
  this->root_=child;
} else if(parent->getLeft() == node) {
  parent->setLeft(child);
} else {
  parent->setRight(child);
}
if(child!=nullptr) {
  child->setParent(parent);
}
delete node;
removeFix(parent, diff);

}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>


// rotate left
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)

{
AVLNode<Key, Value>* rightChild= node->getRight();
  // update parent pointers
  if(node->getParent()==nullptr) {
    this->root_ = rightChild;
    rightChild->setParent(nullptr);
  } else {
    if(node->getParent()->getLeft() == node) {
      node->getParent()->setLeft(rightChild);
    } else {
      node->getParent()->setRight(rightChild);
    }
    rightChild->setParent(node->getParent());
  }
  // rotate
  node->setRight(rightChild->getLeft());
  if(rightChild->getLeft()!=nullptr) {
    rightChild->getLeft()->setParent(node);
  }
  rightChild->setLeft(node);
  node->setParent(rightChild);
}


// rotate right
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* leftChild= node->getLeft();
  // update parent pointers
  if(node->getParent()==nullptr) {
    this->root_ = leftChild;
    leftChild->setParent(nullptr);
  } else {
    if(node->getParent()->getLeft() == node) {
      node->getParent()->setLeft(leftChild);
    } else {
      node->getParent()->setRight(leftChild);
    }
    leftChild->setParent(node->getParent());
  }
  // rotate
  node->setLeft(leftChild->getRight());
  if(leftChild->getRight()!=nullptr) {
    leftChild->getRight()->setParent(node);
  }
  leftChild->setRight(node);
  node->setParent(leftChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
if(parent==nullptr || parent->getParent()==nullptr) {
  return;
}

AVLNode<Key, Value>* grandparent = parent->getParent();

// parent is left child of grandparent
if(grandparent->getLeft()==parent) {
  grandparent->updateBalance(-1);

  if(grandparent->getBalance()==0) {
    return; // tree is balanced
  }

  else if(grandparent->getBalance()==-1) {
    insertFix(grandparent, parent);
  }
  else if(grandparent->getBalance() == -2) {
    if(parent->getLeft()==node) {
      rotateRight(grandparent);
      parent->setBalance(0);
      grandparent->setBalance(0);
    }
    else {
      rotateLeft(parent);
      rotateRight(grandparent);

      if(node->getBalance()==-1){
        parent->setBalance(0);
        grandparent->setBalance(1);
      }
      else if(node->getBalance()==0){
        parent->setBalance(0);
        grandparent->setBalance(0);
      }
      else {
        parent->setBalance(-1);
        grandparent->setBalance(0);
      }
      node->setBalance(0);
    }
  }
} else {
  grandparent->updateBalance(1);
  if(grandparent->getBalance()==0) {
    return;
  }
  else if (grandparent->getBalance()==1) {
    insertFix(grandparent, parent);
  }
  else if(grandparent->getBalance()==2) {
    if(parent->getRight()==node) {
      rotateLeft(grandparent);
      parent->setBalance(0);
      grandparent->setBalance(0);
    } else {

    rotateRight(parent);
    rotateLeft(grandparent);

    if(node->getBalance()==1 ) {
      parent->setBalance(0);
      grandparent->setBalance(-1);
    } else if(node->getBalance()==0) {
      parent->setBalance(0);
      grandparent->setBalance(0);
    } else {
      parent->setBalance(1);
      grandparent->setBalance(0);

    }
    node->setBalance(0);
      }
    }
    }
    }
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
  if(node==nullptr) return;
  AVLNode<Key, Value>* parent = node->getParent();
  int8_t nextDiff = 0;
  if(parent!=nullptr) {
    if(parent->getLeft()==node) {
      nextDiff=1;

    } else {
      nextDiff=-1;
    }
  }
  int8_t newBalance = node->getBalance() + diff;
  if(diff==-1) {
    if(newBalance==-2) {
      AVLNode<Key, Value>* tallerChild = node->getLeft();
      int8_t childBalance = tallerChild->getBalance();

      if(childBalance==-1) {
        rotateRight(node);
        node->setBalance(0);
        tallerChild->setBalance(0);
        removeFix(parent, nextDiff);
      }
      else if(childBalance==0){
        rotateRight(node);
        node->setBalance(-1);
        tallerChild->setBalance(1);
      }
      else {
        AVLNode<Key, Value>* grandchild = tallerChild->getRight();
        int8_t grandchildBalance = grandchild->getBalance();

        rotateLeft(tallerChild);
        rotateRight(node);

        if(grandchildBalance==1) {
          node->setBalance(0);
          tallerChild->setBalance(-1);
        }
        else if(grandchildBalance==0) {
          node->setBalance(0);
          tallerChild->setBalance(0);
        }
        else {
          node->setBalance(1);
          tallerChild->setBalance(0);
        }
        grandchild->setBalance(0);
        removeFix(parent, nextDiff);
      }
    }
    else if(newBalance==-1) {
      node->setBalance(-1);
    }
    else {
      node->setBalance(0);
      removeFix(parent, nextDiff);
    }
  }
  else {
    if(newBalance==2) {
      AVLNode<Key, Value>* tallerChild = node->getRight();
      int8_t childBalance = tallerChild->getBalance();

      if(childBalance==1) {
        rotateLeft(node); 
        node->setBalance(0);
        tallerChild->setBalance(0);
        removeFix(parent, nextDiff);
      }
      else if(childBalance == 0) {
        rotateLeft(node);
        node->setBalance(1);
        tallerChild->setBalance(-1);
      } else {
      AVLNode<Key, Value>* grandchild = tallerChild->getLeft();
      int8_t grandchildBalance = grandchild->getBalance();

      rotateRight(tallerChild); 
      rotateLeft(node);

      if(grandchildBalance == -1) {
        node->setBalance(0);
        tallerChild->setBalance(1);
      }
      else if(grandchildBalance==0) {
        node->setBalance(0);
        tallerChild->setBalance(0);
      }
      else {
        node->setBalance(-1);
        tallerChild->setBalance(0);
      }
      grandchild->setBalance(0);
      removeFix(parent, nextDiff);
    }
  }
  else if(newBalance==1) {
    node->setBalance(1);
  }
  else {
    node->setBalance(0);
    removeFix(parent, nextDiff);
  }
}
}
#endif
