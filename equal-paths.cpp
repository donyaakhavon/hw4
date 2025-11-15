#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
#include <algorithm> // for max
using namespace std;


// You may add any prototypes of helper functions here

int depthGetter(Node* root);


bool equalPaths(Node* root) {

  if(root==nullptr) return true;
  // right doesnt exist so check if left exists
  if(root->right==nullptr) {
    return equalPaths(root->left);
  }

  else if(root->left==nullptr) {
    return equalPaths(root->right);
  }
  else {
    if(depthGetter(root->right)==depthGetter(root->left)) {
      return equalPaths(root->right) && equalPaths(root->left);
    }
    return false; 
  }

}

int depthGetter(Node * root)
{
  if(root==nullptr) return 0;
  int leftDepth = depthGetter(root->left);
  int rightDepth = depthGetter(root->right);
  return (1+ max(leftDepth, rightDepth));

}

