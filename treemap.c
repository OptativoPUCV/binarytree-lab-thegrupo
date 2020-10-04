#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    void* key;
    void * value;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->key = key;
    new->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* nuevo=(TreeMap*)malloc(sizeof(TreeMap));
    nuevo->root=NULL;
    nuevo->current=NULL; 
    nuevo->lower_than=lower_than;
    return nuevo;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode* aux=tree->root;
  TreeNode* parent=NULL;
  while(aux!=NULL){
    parent=aux;
    if(tree->lower_than(key,aux->key)){
      aux=aux->left;
    }else{
      if(tree->lower_than(aux->key,key)){
        aux=aux->right;
      }else{
        return;
      }
    }
  }
  TreeNode* nuevo=createTreeNode(key,value);
  nuevo->parent=parent;
  if(parent==NULL){
    tree->root=nuevo;
  }else{
    if(tree->lower_than(nuevo->key,parent->key)){
      parent->left=nuevo;
    }else{
      parent->right=nuevo;
    }
    tree->current=nuevo;
  }
}

TreeNode * minimum(TreeNode * x){
    while(x->left!=NULL){
      x=x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if((node->right==NULL)&&(node->left==NULL)){//nodo sin hijos
      if(node==node->parent->right){
        node->parent->right=NULL;
      }else{
        node->parent->left=NULL;
      }
    }else{
      if((node->right!=NULL)&&(node->left==NULL)){//nodo con un hijo a la derecha
        if(node==node->parent->left){
          node->parent->left=node->right;
          node->right->parent=node->parent;
        }else{//node==node->parent->right
          node->parent->right=node->right;
          node->right->parent=node->parent;
        }
      }else{
        if((node->right==NULL)&&(node->left!=NULL)){//nodo con un hijo a la izquierda
          if(node==node->parent->right){
            node->parent->right=node->left;
            node->left->parent=node->parent;
          }else{//node==node->parent->left
            node->parent->left=node->left;
            node->left->parent=node->parent;
          }
        }else{//nodo con 2 hujos
          TreeNode* aux=minimum(node->right);
          node->key=aux->key;
          node->value=aux->value;
          if(aux->left!=NULL){
            aux->parent->right=aux->left;
            aux->left->parent=aux->parent;
          }else{
            aux->parent->right=NULL;
          }
          if(node->parent==NULL){
            tree->root=node;
            tree->root->key=node->key;
            tree->root->value=node->value;
          }
          removeNode(tree,aux);
          return;
        }
      }
    }
    free(node);
    return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




void * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* actual=tree->root;
    int flag;
    while(actual!=NULL){
      if(is_equal(tree,actual->key,key)==1){
        tree->current=actual;
        return actual->value;
      }
      flag=tree->lower_than(actual->key,key);
      if(flag==1){
        actual=actual->right;
      }else{
        actual=actual->left;
      }
    }
    return NULL;
}


void * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

void * firstTreeMap(TreeMap * tree) {
    TreeNode* aux=minimum(tree->root);
    return aux->value;
}

void * nextTreeMap(TreeMap * tree) {
    TreeNode* aux;
    if(tree->current->right!=NULL){
      aux=minimum(tree->current->right);
      tree->current=aux;
      return aux->value;
    }else{
      int ban=0;
      aux=tree->current->parent;
      while(aux!=NULL){
        if(tree->lower_than(tree->current->key,aux->key)==1){
          ban=1;
          break;
        }
        aux=aux->parent;
      }
      if(ban==0){
        return NULL;
      }
      tree->current=aux;
      return aux->value;
    }
    return NULL;
}
