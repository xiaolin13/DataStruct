#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "queue.c"
#include "string.h"
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
    node->parent = NULL;
    node->num = 1;
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
    if((*root)->node_list[0] != NULL)
        (*root)->node_list[0]->parent = leftChild;
    leftChild->node_list[1] = (*root)->node_list[1];
    if((*root)->node_list[1] != NULL)
        (*root)->node_list[1]->parent = leftChild;
    rightChild->node_list[0] = (*root)->node_list[2];
    if((*root)->node_list[2] != NULL)
        (*root)->node_list[2]->parent = rightChild;
    rightChild->node_list[1] = (*root)->node_list[3];
    if((*root)->node_list[3] != NULL)
        (*root)->node_list[3]->parent = rightChild;

    top->node_list[0] = leftChild;
    leftChild->parent = top;
    top->node_list[1] = rightChild;
    rightChild->parent = top;

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
    (*now)->num++;
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
    (*now)->num++;
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
    if((*child)->node_list[0] != NULL)
        (*child)->node_list[0]->parent = leftChild;
    leftChild->node_list[1] = (*child)->node_list[1];
    if((*child)->node_list[1] != NULL)
        (*child)->node_list[1]->parent = leftChild;
    rightChild->node_list[0] = (*child)->node_list[2];
    if((*child)->node_list[2] != NULL)
        (*child)->node_list[2]->parent = rightChild;
    rightChild->node_list[1] = (*child)->node_list[3];
    if((*child)->node_list[3] != NULL)
        (*child)->node_list[3]->parent = rightChild;

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
    leftChild->parent = (*now);
    rightChild->parent = (*now);
    return (*now);
}

Result initResult(Node *node, Key i, int tag)
{
    Result r;
    r.pt = node;
    r.i = i;
    r.tag = tag;
    return r;
}

Result searchNode(Node *node, Key key)  //查找
{
    switch(node->flag_node)
    {
        case TWO_LINK:
            return searchTwoNode(node, key);
        case THREE_LINK:
            return searchThreeNode(node, key);
        default:
            return initResult(NULL, -1, 0);
    }
}

Result searchTwoNode(Node *node, Key key)  //从2-节点中查询
{
    int cmp = CompareKey(key, node->key[0]);
    switch(cmp)
    {
        case 1: return searchNode(node->node_list[1], key);
        case -1: return searchNode(node->node_list[0], key);
        case 0: return initResult(node, 0, 1);
    }
}

Result searchThreeNode(Node *node, Key key) // 从3-节点中查询
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
        return initResult(node, 0, 1);
    }
    else if(CompareKey(key, node->key[1]) == 0)
    {
        return initResult(node, 1, 1);
    }
    else 
        return initResult(node, -1, 0);
}

void visit(Node *now)  //访问节点
{
    for(int i = 1; i <= now->num; i++)
    {
        printf("%s ", now->value[i - 1]);
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
        // switch(p->flag_node)
        // {
        //     case TWO_LINK:
        //         if(p->node_list[0] != NULL)
        //             EnQueue(&Q, p->node_list[0]);
        //         if(p->node_list[1] != NULL)
        //             EnQueue(&Q, p->node_list[1]);
        //         break;  //被坑了  专业挖坑
        //     case THREE_LINK:
        //         if(p->node_list[0] != NULL)
        //             EnQueue(&Q, p->node_list[0]);
        //         if(p->node_list[1] != NULL)
        //             EnQueue(&Q, p->node_list[1]);
        //         if(p->node_list[2] != NULL)
        //             EnQueue(&Q, p->node_list[2]);
        //         break;
        // }

        switch(p->num)
        {
            case 1:
                if(p->node_list[0] != NULL)
                    EnQueue(&Q, p->node_list[0]);
                if(p->node_list[1] != NULL)
                    EnQueue(&Q, p->node_list[1]);
                break;  //被坑了  专业挖坑
            case 2:
               if(p->node_list[0] != NULL)
                    EnQueue(&Q, p->node_list[0]);
                if(p->node_list[1] != NULL)
                    EnQueue(&Q, p->node_list[1]);
                if(p->node_list[2] != NULL)
                    EnQueue(&Q, p->node_list[2]);
                break;  //被坑了  专业挖坑 
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

int btree_delete(Node** tree, Key key)  //寻找匹配的关键字 为下一步准备
{
    int idx = 0;
    Node* node = (*tree);

    while(NULL != node)
    {
        for(idx = 0; idx < node->num; idx++)
        {
            if(CompareKey(key, node->key[idx]) == 0)
                return delete_btree(tree, node ,idx);
            else if(CompareKey(key, node->key[idx]) == -1)
                break;
        }

        node = node->node_list[idx];
    }
    return 0;
}

int delete_btree(Node** tree, Node* node, int idx)  //找到了匹配的节点 开始删除
{
    Node* orig = node, *child = node->node_list[idx]; 

    while(NULL != child)  //求直接前继节点
    {
        node = child;
        child = node->node_list[child->num];
    }

    orig->key[idx] = node->key[node->num - 1];  //赋值
    orig->value[idx] = node->value[node->num - 1];

    node->key[--node->num] = -1;   //先删除值   节点个数也减一
    node->value[node->num] = (char*)"";

    if(node->num < min)   //2-3 树  
    {
        return btree_merge(tree, node);
    }
    return 0;
}

int btree_merge(Node** tree, Node *node)
{
    int idx = 0, m = 0, mid = 0;
    Node* parent = node->parent, *right = NULL, *left = NULL;

    if(NULL == parent)  //node 是根节点   不必进行合并处理
    {
        if(0 == node->num)   //如果根节点 为空 就把第一个子链接放上来
        {                               //为什么感觉不会发生这种情况
            if(NULL != node->node_list[0])    //待会删掉看看
            {
                (*tree) = node->node_list[0];
                node->node_list[0]->parent = NULL;
            }
            else 
            {
                (*tree) = NULL;
            }
            free(node);
        }
        return 0;
    }

    for(idx = 0; idx <= parent->num; idx++)  //查找 node 是其父节点的第几个孩子节点
    {
        if(parent->node_list[idx] == node)
        {
            break;
        }
    }

    if(idx > parent->num)  //如果范围超标  则直接退出
    {
        printf("Error!\n");
        exit(-1);
    }
    else if(idx == parent->num)  //最后一个孩子节点  (left < node)
    {
        mid = idx - 1;
        left = parent->node_list[mid];

        if((node->num + left->num + 1) <= max)  //两者节点数不够
            return merge_btree(tree, left, node, mid);

        for(m = node->num; m > 0; m--)  //这一步其实也可以不用了
        {
            node->key[m] = node->key[m - 1];
            node->value[m] = node->value[m - 1];
            node->node_list[m + 1] = node->node_list[m];
        }
        node->node_list[1] = node->node_list[0];
        node->key[0] = parent->key[mid];
        node->value[0] = parent->value[mid];
        node->num++;

        if(NULL != left->node_list[left->num])
        {
            left->node_list[left->num]->parent = parent->parent;
        }

        parent->key[mid] = left->key[left->num - 1];
        parent->value[mid] = left->value[left->num - 1];
        left->key[left->num - 1] = -1;
        left->value[left->num - 1] = (char*)"";
        left->node_list[left->num - 1] = NULL;
        left->num--;
        return 0;
    }

    //非最后一个孩子节点
    mid = idx;
    right = parent->node_list[mid + 1];
    if((node->num + right->num + 1) <= max)
    {
        return merge_btree(tree, node, right, mid);
    }

    node->key[node->num] = parent->key[mid];
    node->value[node->num++] = parent->value[mid];
    node->node_list[node->num] = right->node_list[0];

    parent->key[mid] = right->key[0];
    parent->value[mid] = right->value[0];

    for(m = 0; m < right->num; m++)
    {
        right->key[m] = right->key[m + 1];
        right->value[m] = right->value[m + 1];
        right->node_list[m] = right->node_list[m + 1];
    }
    right->node_list[m] = NULL;
    right->num--;
    return 0;
}

int merge_btree(Node** tree, Node *left, Node *right, int mid)
{
    int m = 0;
    Node* parent = left->parent;
    left->key[left->num] = parent->key[mid];
    left->value[left->num++] = parent->value[mid];

    for(int i = 0, j = left->num; i < right->num; i++, j++)
    {
        left->key[j] = right->key[i];
        left->value[j] = right->value[i];
    }
    for(int i = 0, j = left->num; i < right->num + 1; i++, j++)
    {
        left->node_list[j] = right->node_list[i];
    }

    for(m = 0; m <= right->num; m++)
    {
        if(NULL != right->node_list[m])
        {
            right->node_list[m]->parent = left;
        }
    }
    left->num += right->num;

    for(m = mid; m < parent->num - 1; m++)
    {
        parent->key[m] = parent->key[m + 1];
        parent->value[m] = parent->value[m + 1];
        parent->node_list[m + 1] = parent->node_list[m + 2];
    }

    parent->key[m] = -1;
    parent->value[m] = (char*)"";
    parent->node_list[m + 1] = NULL; 
    parent->num--;
    free(right);

    if(parent->num < min)
        return btree_merge(tree, parent);
    return 0;
}