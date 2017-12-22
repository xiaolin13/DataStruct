#ifndef _RBTREE_H_
#define _RBTREE_H_

#define RED 0
#define BLACK 1

typedef int Type;

typedef struct RBTreeNode {
    unsigned char color;
    Type key;
    struct RBTreeNode *left;
    struct RBTreeNode *right;
    struct RBTreeNode *parent;
} Node, *RBTree;

typedef struct rb_root {
    Node *node;
} RBRoot;

RBRoot* create_rbtree();

void destroy_rbtree(RBRoot *root);

int insert_rbtree(RBRoot *root, Type key);

void delete_rbtree(RBRoot *root);

void preorder_rbtree(RBRoot *root);

void inorder_rbtree(RBRoot *root);

void postorder_rbtree(RBRoot *root);

int rbtree_search(RBRoot *root, Type key);

int iterative_rbtree_search(RBRoot *root, Type key);

int rbtree_minimum(RBRoot *root, int *val);

int rbtree_maximum(RBRoot *root, int *val);

void print_rbtree(RBRoot *root);

#endif