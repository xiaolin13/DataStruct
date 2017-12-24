#ifndef _ALL_H
#define _ALL_H

#define TWO_LINK 2
#define THREE_LINK 3
#define FOUR_LINK 4

#define THREE 3
#define FOUR 4

#define true 1
#define false 0
#define bool int  //不确定有没有定义bool 只能先这样放着

#define max 2
#define min 1

typedef int Key;
typedef char* String;

typedef struct Node 
{
    Key *key;   //存放值（一个节点最多三个值）
    String *value;
    Node **node_list;  //存放指向子节点的链接（最多四条）
    int flag_node;  //区分2- 3-节点的标志位
    Node *parent;  //为了删除作准备
    int num;  //元素个数
}Node;

//队列节点
typedef struct qNode 
{
    Node* member;
    struct qNode* next;
}qNode, *pNode;

//队列定义
typedef struct Queue
{
    pNode head;
    pNode tail;
}Queue, *pQueue;

typedef struct Result {
    Node *pt;
    int i;
    int tag;   //查找成功的标志
}Result;

#endif 