#ifndef _ALL_H
#define _ALL_H

typedef int Type; //默认 整数类型   要修改时改这里 即可

//二叉树 节点
typedef struct AVLTreeNode {   
    Type key;
    int height;
    struct AVLTreeNode *lchild;
    struct AVLTreeNode *rchild;
}AVLNode, *AVLTree;


//栈 和 队列节点
typedef struct Node 
{
    AVLNode* member;
    struct Node* next;
}Node, *pNode;

//队列定义
typedef struct Queue
{
    pNode head;
    pNode tail;
}Queue, *pQueue;



























//栈定义
typedef struct Stack
{
    pNode Top;
    pNode Buttom;
    int num;
}Stack, *pStack;

#endif 