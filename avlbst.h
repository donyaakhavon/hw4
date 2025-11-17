#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

// forgot to write comments so im unsubmitting :( 

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
    // standard bst insertion logic. walk down tree until we find correct spot. if key exists, simply update new value. 
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
// parent is the place where the new node attaches. create the avl node and link it under the parent accordingly. 
while(current!=nullptr) {
  parent = current;
  if(new_item.first == parent->getKey()) {
    // key already present, update and stop
    parent->setValue(new_item.second);
    return;
  }
  // decide left vs right child based on key comparison
  current = (new_item.first < parent->getKey()) 
              ? parent->getLeft()
              : parent->getRight(); // using ternary pattern for clarity and to avoid nested if statements    

}
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(new_item.first<parent->getKey()) {
      parent->setLeft(newNode);
    } else {
      parent->setRight(newNode);
    }
    newNode->setBalance(0); // new leaf is balanced at creation

    // if parent was perfectly balanced, adding a child tilts it to -1 or + 1
    if(parent->getBalance()==0){
      int8_t direction = (parent->getLeft() == newNode ? -1 : 1);
      parent->setBalance(direction);
      insertFix(parent, newNode);
    }
    else {
      parent->setBalance(0);
    }
}
 



template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(this->root_ == nullptr) {
      return;
    }
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    if(node == nullptr) {
      return;
    }


    // if 2 children, replace with predecessor and rebalance 
    if(node->getRight() != nullptr && node->getLeft() != nullptr) {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
      nodeSwap(pred, node);
      // delete node;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    int8_t diff = 0; 

    if(parent != nullptr) {
        if (parent->getLeft() == node) { 
            diff = 1;
        }
        else {
            diff = -1;
        }
    }

    // if no children, remove
    if(node->getRight() == nullptr && node->getLeft() == nullptr) {
      // if its the root
      if(node==this->root_) {
        delete node;
        this->root_ = nullptr;
      }
      else if (parent != nullptr) {
        if(parent->getLeft() == node) {
            parent->setLeft(nullptr);
            delete node;
        }
        else {
            parent->setRight(nullptr);
            delete node;
        }
      }
    }

    // if there's only a left child
    else if (node->getRight() == nullptr ) { 
      // && node->getRight() == nullptr
      AVLNode<Key, Value>* child = node->getLeft();
        if(node==this->root_) {
          this->root_ = child;
          child->setParent(nullptr); 
        }
        else {
          if (parent->getRight() == node) {
            parent->setRight(child);
          }
          else {
            parent->setLeft(child);
          }
          child->setParent(parent);
        }
      delete node;
    }

    // if there's only a right child
    else if (node->getLeft() == nullptr) { // 
      AVLNode<Key, Value>* child = node->getRight();
        if(node==this->root_) {
          this->root_ = child;
          child->setParent(nullptr);
        }
        else {
          if (parent->getLeft() == node) {
            parent->setLeft(child);
          }
          else {
            parent->setRight(child);
          }
          child->setParent(parent); 
        }
      delete node;
    } 
  
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
    AVLNode<Key, Value>* gp = parent->getParent();
    if (gp == nullptr) {
        return;
    }
// determine whether subtree grew on left or right side
int8_t shift = (gp->getLeft() == parent ? -1 : 1);
gp->updateBalance(shift);
// snapshot of the updated greandparent balance
int8_t bal = gp->getBalance();
// case 1, balance == 1 height increased but subtree still valid.

if(bal == -1 || bal == 1) {
  // bubble up
  insertFix(gp, parent);
  return;
}
// case 2: balance == 0
if(bal == 0) {
  return;
}
// case 3: unbalanced left side, gp balance == -2
if(bal == -2) {
  bool isLL = (gp->getLeft()==parent) && (parent->getLeft() == node);
  if(isLL) {
    // LL rotation
    rotateRight(gp);
    parent->setBalance(0);
    gp->setBalance(0);
  }
  else {
    // LR rotation
    rotateLeft(parent);
    rotateRight(gp);
    int8_t ndBal = node->getBalance();
    if(ndBal==-1){
      parent->setBalance(0);
      gp->setBalance(1);
    } else if(ndBal==0) {
      parent->setBalance(0);
      gp->setBalance(0);
    } else {
      parent->setBalance(-1);
      gp->setBalance(0);
    }
    node->setBalance(0);
  }
  return;
}
    // case 4: unbalanced right side up
  if(bal == 2) {
    bool isRR = (gp->getRight()==parent) && (parent->getRight() == node);
    if(isRR) {
      rotateLeft(gp);
      parent->setBalance(0);
      gp->setBalance(0);
      } 
      else {
      rotateRight(parent);
      rotateLeft(gp);
      int8_t ndBal = node->getBalance();
      if (ndBal==1) {
        parent->setBalance(0);
        gp->setBalance(-1);
      } else if (ndBal==0){
        parent->setBalance(0);
        gp->setBalance(0);
      }
      else {
        parent->setBalance(1);
        gp->setBalance(0);
      }
      node->setBalance(0);
    }
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix( AVLNode<Key,Value>* node, int8_t diff) {

  if(node == nullptr) {
    return;
  }

  AVLNode<Key, Value>* parent = node->getParent();

  // determine whether node is left or right child relative to parent
  int8_t nextDir = 0;
  if(parent!=nullptr) {
    nextDir = (parent->getLeft() == node ? 1 : -1);
  }
  // precompute updated balance 
  int8_t updated = node->getBalance() + diff;

// case 2
  if(updated == -2) {
    AVLNode<Key, Value>* leftSub = node->getLeft();
    int8_t lb = leftSub->getBalance();

     if(lb <=0) {
      rotateRight(node);

      // if child was neutral, rotation shortens height only one way
      if(lb ==0) {
        node->setBalance(-1);
        leftSub->setBalance(1);
        return;
      }
      node->setBalance(0);
      leftSub->setBalance(0);
      removeFix(parent, nextDir);
     }
     else {
      AVLNode<Key, Value>* mid = leftSub->getRight();
      int8_t mb = mid->getBalance();
      rotateLeft(leftSub);
      rotateRight(node);

      if(mb==-1) {
        node->setBalance(1);
        leftSub->setBalance(0);
      }
      else if (mb ==0) {
        node->setBalance(0);
        leftSub->setBalance(0);
      }
      else {
        node->setBalance(0);
        leftSub->setBalance(-1);
      }
      mid->setBalance(0);
      removeFix(parent, nextDir);
     }
     return;
  }

  // case 2 (+2)
  if(updated == 2) {
    AVLNode<Key, Value>* rightSub = node->getRight();
    int8_t rb = rightSub->getBalance();
    if(rb>=0) {
      rotateLeft(node);
      if(rb==0){
        node->setBalance(1);
        rightSub->setBalance(-1);
        return;
      }
      node->setBalance(0);
      rightSub->setBalance(0);
      removeFix(parent, nextDir);
    }
    else {
      AVLNode<Key,Value>* mid = rightSub->getLeft();
      int8_t mb = mid->getBalance();
      rotateRight(rightSub);
      rotateLeft(node);

      if(mb==1){
        node->setBalance(-1);
        rightSub->setBalance(0);
      } else if (mb==0) {
        node->setBalance(0);
        rightSub->setBalance(0);
      } else {
        node->setBalance(0);
        rightSub->setBalance(1);
      }
      mid->setBalance(0);
      removeFix(parent, nextDir);
    }
    return;
  }
  node->setBalance(updated);
  // stop if new balance is non zero, means height did not shrink anymore
  if (updated!=0) {
    return;
  }
  removeFix(parent, nextDir);
}




#endif
