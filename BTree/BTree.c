#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "queue.c"

//初始化B树
Node* initBTree(Key key, String value)
{
    Node *node;  //初始化一个  根节点
    node = (Node *)malloc(sizeof(Node));  //动态分配内存 内存分配失败返回NULL
    if(node == NULL)
    {
        printf("Node: Dynamically allocated memory error\n");
        return NULL;
    }
    node->flag_node = TWO_LINK;   //初始化是2-节点
    node->key = (Key *)malloc(THREE * sizeof(Key));  //索引   
    if(node->key == NULL)  
    {
        printf("Key: Dynamically allocated memory error\n");
        return NULL;
    }
    node->value = (String *)malloc(THREE * sizeof(String));  //值
    if(node->value == NULL)
    {
        printf("String(value): Dynamically allocated memory error\n");
        return NULL;
    }
    node->key[0] = key;
    node->value[0] = value;
    node->node_list = (Node **)malloc(FOUR * sizeof(Node*));  //子链接
    if(node->node_list == NULL)
    {
        printf("Node_list: Dynamically allocated memory error\n");
        return NULL;
    }
    for(int i = 0; i < FOUR; i++)
    {
        node->node_list[i] = NULL;
    }
    return node;  
}

//将节点 放入树中
bool putNode(Node **node, int key, String value)
{
    if(put_node(node, key, value, 0) != NULL)  //间接调用
        return true;
    else 
        return false;
}

//为处理2- 和 3- 节点作转接
Node* put_node(Node **node ,Key key, String value, int depth)
{
    Node *child_node;
    if((*node) == NULL)
        return NULL;
    switch((*node)->flag_node)
    {
        case TWO_LINK:   //按2-节点插入的方法插入
            child_node = put_two(node, key, value, depth + 1);
            break;
        case THREE_LINK:  //按3-节点插入的方法插入
            child_node = put_three(node, key, value, depth + 1);
            break;
        default:   //不匹配 
            return NULL;
    }
    //还差一步  处理节点deal_node
    return deal_links(&child_node, node, key, value, depth);
}

//比较索引大小， 考虑到以后 Key类型改变后 方便修改才写
int CompareKey(Key key1, Key key2)  //现在的索引值是 int 类型
{
    if(key1 > key2)
        return 1;
    else if(key1 < key2)
        return -1;
    else 
        return 0;
}

//放入 2- 节点中
Node* put_two(Node **node ,Key key, String value, int depth)
{
    int cmp = CompareKey(key, (*node)->key[0]);  //根据比较值选择插入到哪条子链接
    switch(cmp)
    {
        case 1: return put_node(&((*node)->node_list[1]),  key, value, depth); break;
        case 0: (*node)->value[0] = value; return *node; break;
        case -1: return put_node(&((*node)->node_list[0]), key, value ,depth); break;
        default: return NULL;
    }

    return NULL;  //其实可以不用写的
}

//再3-节点中   查找插入  
Node* put_three(Node **node, Key key, String value, int depth)
{
    if(CompareKey(key, (*node)->key[0]) != 1)   //根据索引值选择插入子链接
        return put_node(&((*node)->node_list[0]), key, value, depth);
    else if(CompareKey(key, (*node)->key[0]) == 1 && CompareKey(key, ((*node)->key[1])) != 1)
        return put_node(&((*node)->node_list[1]), key, value, depth);
    else if(CompareKey(key, ((*node)->key[1])) == 1)
        return put_node(&((*node)->node_list[2]), key, value, depth);
    else 
        return NULL;
}

Node* deal_links(Node **child, Node **now, Key key, String value, int depth)
{
    if(*child == NULL)
    {
        switch((*now)->flag_node)
        {
            case TWO_LINK:
                addTwoNode(now, key, value);
                break;
            case THREE_LINK:
                addThreeNode(now, key, value);
                break;
            default:
                return NULL;
        }
        if(depth == 0 && (*now)->flag_node == FOUR_LINK)
            dealTopTree(now);
        return (*now);
    }
    if((*child)->flag_node == FOUR_LINK)
        dealFourLinks(child, now);
    if(depth == 0 && (*now)->flag_node == FOUR_LINK)
        dealTopTree(now);

    return (*now);
}

void dealTopTree(Node **root)  //处理头节点  头节点不可能出现4-节点
{
    Node *top;
    Node *leftChild;
    Node *rightChild;
    top = initBTree((*root)->key[1], (*root)->value[1]);
    leftChild = initBTree((*root)->key[0], (*root)->value[0]);
    rightChild = initBTree((*root)->key[2], (*root)->value[2]);

    leftChild->node_list[0] = (*root)->node_list[0];
    leftChild->node_list[1] = (*root)->node_list[1];
    rightChild->node_list[0] = (*root)->node_list[2];
    rightChild->node_list[1] = (*root)->node_list[3];

    top->node_list[0] = leftChild;
    top->node_list[1] = rightChild;

    (*root) = top;
}

//加入到2-节点
void addTwoNode(Node **now, Key key, String value)
{
    (*now)->flag_node = THREE_LINK;

    if(CompareKey(key, (*now)->key[0]) == 0)  //当和 2-节点中的索引值相等时   更新value值
    {                                          //然后回复 2-状态， 然后返回
        (*now)->flag_node = TWO_LINK;
        (*now)->value[0] = value;
        return;
    }

    if(CompareKey(key, (*now)->key[0]) == -1)    //寻找地方插入
    {
        (*now)->key[1] = (*now)->key[0];
        (*now)->value[1] = (*now)->value[0];
    }
    int location = (CompareKey(key, (*now)->key[0]) == 1) ? 1 : 0;
    (*now)->key[location] = key;
    (*now)->value[location] = value;
}

//加入到3-节点中
void addThreeNode(Node **now, Key key, String value)  
{
    (*now)->flag_node = FOUR_LINK;   //插入后就变成4-节点了  

    if(CompareKey(key, (*now)->key[0]) == -1)    //寻找位置插入  根据索引值大小
    {
        (*now)->key[2] = (*now)->key[1];
        (*now)->value[2] = (*now)->value[1];
        (*now)->key[1] = (*now)->key[0];
        (*now)->value[1] = (*now)->value[0];
        (*now)->key[0] = key;
        (*now)->value[0] = value;
    }
    else if((CompareKey(key, (*now)->key[0]) == 1) && (CompareKey(key, (*now)->key[1]) == -1)) 
    {
        (*now)->key[2] = (*now)->key[1];
        (*now)->value[2] = (*now)->value[1];
        (*now)->key[1] = key;
        (*now)->value[1] = value;
    }
    else if(CompareKey(key, (*now)->key[1]) == 1)  
    {
        (*now)->key[2] = key;
        (*now)->value[2] = value;
    }
    else if(CompareKey(key, (*now)->key[0]) == 0)  //当遇到相同索引值时，都只是更新value 然后将
    {                                                       //状态恢复为 3- 节点状态
        (*now)->flag_node = THREE_LINK;
        (*now)->value[0] = value;
    }
    else if(CompareKey(key, (*now)->key[1]) == 0)
    {
        (*now)->flag_node = THREE_LINK;
        (*now)->value[1] = value;
    }
}

Node* dealFourLinks(Node **child, Node **now) // 处理4-节点 只允许暂时存在
{
    if((*now)->flag_node == TWO_LINK)
        addTwoNode(now, (*child)->key[1], (*child)->value[1]);
    else if((*now)->flag_node == THREE_LINK)
        addThreeNode(now, (*child)->key[1], (*child)->value[1]);
    Node* leftChild = initBTree((*child)->key[0], (*child)->value[0]);
    Node* rightChild = initBTree((*child)->key[2], (*child)->value[2]);
    leftChild->node_list[0] = (*child)->node_list[0];
    leftChild->node_list[1] = (*child)->node_list[1];
    rightChild->node_list[0] = (*child)->node_list[2];
    rightChild->node_list[1] = (*child)->node_list[3];

    if((*child) == (*now)->node_list[0])
    {
        (*now)->node_list[3] = (*now)->node_list[2];
        (*now)->node_list[2] = (*now)->node_list[1];
        (*now)->node_list[0] = leftChild;
        (*now)->node_list[1] = rightChild;
    }
    else if((*child) == (*now)->node_list[1])
    {
        (*now)->node_list[3] = (*now)->node_list[2];
        (*now)->node_list[1] = leftChild;
        (*now)->node_list[2] = rightChild;
    }
    else if((*child) == (*now)->node_list[2])
    {
        (*now)->node_list[2] = leftChild;
        (*now)->node_list[3] = rightChild;
    } 
    free(*child);
    return (*now);
}

String searchNode(Node *node, int key)  //查找
{
    switch(node->flag_node)
    {
        case TWO_LINK:
            return searchTwoNode(node, key);
        case THREE_LINK:
            return searchThreeNode(node, key);
        default:
            return NULL;
    }
}

String searchTwoNode(Node *node, int key)  //从2-节点中查询
{
    int cmp = CompareKey(key, node->key[0]);
    switch(cmp)
    {
        case 1: return searchNode(node->node_list[1], key);
        case -1: return searchNode(node->node_list[0], key);
        case 0: return node->value[0];
    }
}

String searchThreeNode(Node *node, int key) // 从3-节点中查询
{
    if((CompareKey(key, node->key[0])) == -1)
    {
        return searchNode(node->node_list[0], key);
    }
    else if((CompareKey(key, node->key[0]) == 1) && (CompareKey(key, node->key[1]) == -1))
    {
        return searchNode(node->node_list[1], key);
    }
    else if(CompareKey(key, node->key[1]) == 1)
    {
        return searchNode(node->node_list[2], key);
    }
    else if(CompareKey(key, node->key[0]) == 0)
    {
        return node->value[0];
    }
    else if(CompareKey(key, node->key[1]) == 0)
    {
        return node->value[1];
    }
    else 
        return NULL;
}

void visit(Node *now)  //访问节点
{
    switch(now->flag_node)
    {
        case TWO_LINK: 
            printf("%s ", now->value[0]);
            break;
        case THREE_LINK:
            printf("%s ", now->value[0]);
            printf("%s ", now->value[1]);
            break;
        default:
            printf("Error");
    }
}

void levelTraverse(Node *now)  //层次遍历B树
{
    Queue Q;
    InitQueue(&Q);
    Node *p;
    EnQueue(&Q, now);
    while(!isQueueEmpty(&Q))  //不断入队 出队
    {
        p = DeQueue(&Q);
        visit(p);
        switch(p->flag_node)
        {
            case TWO_LINK:
                if(p->node_list[0] != NULL)
                    EnQueue(&Q, p->node_list[0]);
                if(p->node_list[1] != NULL)
                    EnQueue(&Q, p->node_list[1]);
                break;  //被坑了
            case THREE_LINK:
                if(p->node_list[0] != NULL)
                    EnQueue(&Q, p->node_list[0]);
                if(p->node_list[1] != NULL)
                    EnQueue(&Q, p->node_list[1]);
                if(p->node_list[2] != NULL)
                    EnQueue(&Q, p->node_list[2]);
                break;
        }
    }
    printf("\n");
}

String minimunValue(Node *root) //查找最小索引对应的值
{
    Node *y = root;
    if(root == NULL)
        return NULL;
    while(NULL != y->node_list[0])
        y = y->node_list[0];
    return y->value[0];
}

String maximunValue(Node *root)  // 查找最大索引对应的值
{
    Node *y = root;
    if(root == NULL)
        return NULL;
    while(NULL != y->node_list[y->flag_node - 1])  //不停找下去
        y = y->node_list[y->flag_node - 1];
    return y->value[y->flag_node - 2];
}