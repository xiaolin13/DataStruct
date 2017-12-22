#include <stdio.h>
#include <stdlib.h>
#include "avlTree.h"
#include "stack.c"
#include "queue.c"

#define HEIGHT(p) ((p == NULL) ? -1 : (((AVLNode *)(p))->height))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void initAVLTree(AVLTree tree) //初始化  平衡树
{                              //有待 改正  一开始初始化不知道要不要赋值
    tree = NULL;
    //初始化 tree 为 空
}

AVLNode *avlTreeMinimum(AVLTree tree) //求得平衡树 最小值
{
    if (tree == NULL) //一直往左子树搜寻下去
        return NULL;
    while (tree->lchild != NULL)
        tree = tree->lchild;
    return tree;
}

AVLNode *avlTreeMaximum(AVLTree tree) //求得平衡树  最大值
{
    if (tree == NULL) //一直沿右子树搜寻下去
        return NULL;
    while (tree->rchild != NULL)
        tree = tree->lchild;
    return tree;
}

void visit(AVLNode *node) //访问节点的 key  以后修改类型也可以从这里入手
{
    printf("%d ", node->key);
}

void visitRoot(AVLNode *node) //访问并打印出具体信息
{
    printf("%2d is root\n", node->key);
}

void visitChild(AVLNode *node, Type key, int direction)
{
    printf("%2d is %2d's %6s child\n", node->key, key, direction == 1 ? "right" : "left");
}

void preOrderAVLTree(AVLTree tree) //递归前序遍历平衡树
{
    if (tree != NULL)
    {
        visit(tree);
        preOrderAVLTree(tree->lchild);
        preOrderAVLTree(tree->rchild);
    }
}

void preNoRecursive(AVLTree tree) //非递归前序遍历 使用栈
{
    if (tree == NULL)
        return;
    AVLTree p = tree;
    Stack s;
    InitStack(&s);
    while (!isStackEmpty(&s) || p)
    {
        if (p)
        {
            visit(p);
            Push(&s, p);
            p = p->lchild;
        }
        else
        {
            p = Pop(&s);
            p = p->rchild;
        }
    }
}

void inOrderAVLTree(AVLTree tree) //中序遍历平衡树
{
    if (tree != NULL)
    {
        inOrderAVLTree(tree->lchild);
        visit(tree);
        inOrderAVLTree(tree->rchild);
    }
}

void inNoRecursive(AVLTree tree) //非递归 中序遍历
{
    if (tree == NULL)
        return;
    AVLTree p = tree;
    Stack s;
    InitStack(&s);
    while (!isStackEmpty(&s) || p)
    {
        if (p)
        {
            Push(&s, p);
            p = p->lchild;
        }
        else
        {
            p = Pop(&s);
            visit(p);
            p = p->rchild;
        }
    }
}

void postOrderAVLTree(AVLTree tree) //后序遍历平衡树
{
    if (tree != NULL)
    {
        postOrderAVLTree(tree->lchild);
        postOrderAVLTree(tree->rchild);
        visit(tree);
    }
}

void postNoRecursive(AVLTree tree) //非递归 后序遍历
{
    if (tree == NULL)
        return;
    Stack s1, s2;
    InitStack(&s1);
    InitStack(&s2);
    AVLNode *node = tree;
    Push(&s1, node);
    while (!isStackEmpty(&s1))
    {
        node = Pop(&s1);
        Push(&s2, node);
        if (node->lchild)
            Push(&s1, node->lchild);
        if (node->rchild)
            Push(&s1, node->rchild);
    }
    while (!isStackEmpty(&s2))
    {
        node = Pop(&s2);
        visit(node);
    }
}

void printAVLTreeByLevel(AVLTree tree) //层次遍历平衡树
{
    Queue Q;
    InitQueue(&Q);
    AVLTree p;
    EnQueue(&Q, tree);
    while (!isQueueEmpty(&Q))
    {
        p = DeQueue(&Q);
        visit(p);
        if (p->lchild != NULL)
            EnQueue(&Q, p->lchild);
        if (p->rchild != NULL)
            EnQueue(&Q, p->rchild);
    }
    printf("\n");
}

AVLNode *searchAVLTree(AVLTree tree, Type key) //递归查找
{
    if (tree == NULL || tree->key == key)
        return tree;
    if (less(tree->key, key))
        return searchAVLTree(tree->rchild, key);
    else
        return searchAVLTree(tree->lchild, key);
}

AVLNode *noRecursiveSearch(AVLTree tree, Type key) //非递归 查找
{
    while ((tree != NULL) && (tree->key != key))
    {
        if (less(key, tree->key))
            tree = tree->lchild;
        else
            tree = tree->rchild;
    }
    return tree;
}

int AVLTreeHeight(AVLTree tree) //返回树的高度
{
    return tree->height;
}

void printAVLTree(AVLTree tree, Type key, int direction) //打印出具体信息
{
    if (tree != NULL)
    {
        if (direction == 0)
            visitRoot(tree);
        else
            visitChild(tree, key, direction);

        printAVLTree(tree->lchild, tree->key, -1);
        printAVLTree(tree->rchild, tree->key, 1);
    }
}

void destroyAVLTree(AVLTree tree) //销毁二叉树
{
    if (tree == NULL)
        return;
    if (tree->lchild != NULL) //左子树 非空 就先递归销毁 左子树
        destroyAVLTree(tree->lchild);
    if (tree->rchild != NULL)
        destroyAVLTree(tree->rchild);
    free(tree);
}

AVLNode *leftLeftRotation(AVLTree k2) //左旋转 顺时针旋转
{
    AVLTree k1;
    k1 = k2->lchild;
    k2->lchild = k1->rchild;
    k1->rchild = k2;

    k2->height = MAX(HEIGHT(k2->lchild), HEIGHT(k2->rchild)) + 1;
    k1->height = MAX(HEIGHT(k1->lchild), k2->height) + 1;

    return k1;
}

AVLNode *rightRightRotation(AVLTree k1) //右旋转 逆时针旋转
{
    AVLTree k2;
    k2 = k1->rchild;
    k1->rchild = k2->lchild;
    k2->lchild = k1;

    k1->height = MAX(HEIGHT(k1->lchild), HEIGHT(k1->rchild)) + 1;
    k2->height = MAX(HEIGHT(k2->rchild), k1->height) + 1;

    return k2;
}

AVLNode *leftRightRotation(AVLTree k2) //先右旋 左子节点 ， 然后再 左旋转 头节点
{
    k2->lchild = rightRightRotation(k2->lchild);
    return leftLeftRotation(k2);
}

AVLNode *rightLeftRotation(AVLTree k2) //先左旋 右子节点， 然后 再 右旋 头节点
{
    k2->rchild = leftLeftRotation(k2->rchild);
    return rightRightRotation(k2);
}

AVLNode *createAVLNode(Type key, AVLNode *lchild, AVLNode *rchild)
//创建一个AVLNode 节点
{
    AVLNode *p;
    if ((p = (AVLNode *)malloc(sizeof(AVLNode))) == NULL)
        return NULL;
    p->key = key;
    p->height = 0;
    p->lchild = lchild;
    p->rchild = rchild;
    return p;
}

//插入节点 就要使用 旋转操作了
AVLNode *insertAVLTree(AVLTree tree, Type key) //插入节点
{
    if (tree == NULL)
    {
        tree = createAVLNode(key, NULL, NULL);
        if (tree == NULL)
        {
            printf("ERROR: cretea avltree node failed!\n");
            return NULL;
        }
    }
    else if (key < tree->key)
    {
        tree->lchild = insertAVLTree(tree->lchild, key);
        if ((HEIGHT(tree->lchild) - (HEIGHT(tree->rchild))) == 2)
        {
            if (key < tree->lchild->key)
                tree = leftLeftRotation(tree);
            else
                tree = leftRightRotation(tree);
        }
    }
    else if (key > tree->key)
    {
        tree->rchild = insertAVLTree(tree->rchild, key);
        if (((HEIGHT(tree->rchild) - HEIGHT(tree->lchild))) == 2)
        {
            if (key > tree->rchild->key)
                tree = rightRightRotation(tree);
            else
                tree = rightLeftRotation(tree);
        }
    }
    else
    {
        printf("add error: no allow to add same key!\n");
    }
    tree->height = MAX(HEIGHT(tree->lchild), HEIGHT(tree->rchild)) + 1;
    return tree;
}

AVLNode *deleteAVLNode(AVLTree tree, AVLNode *z) //删除节点
{                                                //左边删除就从 左边 节点动手
    if (tree == NULL || z == NULL)               //删除之后要恢复平衡性
        return NULL;
    if (z->key < tree->key)
    {
        tree->lchild = deleteAVLNode(tree->lchild, z);
        if ((HEIGHT(tree->rchild) - HEIGHT(tree->lchild)) == 2)
        {
            AVLNode *r = tree->rchild;
            if (HEIGHT(r->lchild) > HEIGHT(r->rchild))
                tree = rightLeftRotation(tree);
            else
                tree = rightRightRotation(tree);
        }
    }
    else if (z->key > tree->key)
    {
        tree->rchild = deleteAVLNode(tree->rchild, z);
        if ((HEIGHT(tree->lchild) - HEIGHT(tree->rchild)) == 2)
        {
            AVLNode *l = tree->lchild;
            if ((HEIGHT(l->rchild) - HEIGHT(l->rchild)) == 2)
                tree = leftRightRotation(tree);
            else
                tree = leftLeftRotation(tree);
        }
    }
    else
    {
        if ((tree->rchild) && (tree->lchild))
        {
            if (HEIGHT(tree->lchild) > HEIGHT(tree->rchild))
            {
                AVLNode *max = avlTreeMaximum(tree->lchild);
                tree->key = max->key;
                tree->lchild = deleteAVLNode(tree->lchild, max);
            }
            else
            {
                AVLNode *min = avlTreeMinimum(tree->rchild);
                tree->key = min->key;
                tree->lchild = deleteAVLNode(tree->rchild, min);
            }
        }
        else
        {
            AVLNode *tmp = tree;
            tree = tree->lchild ? tree->lchild : tree->rchild;
            free(tmp);
        }
    }
    tree->height = MAX(HEIGHT(tree->lchild), HEIGHT(tree->rchild)) + 1; //保持 树的高度
    return tree;
}

AVLNode *deleteAVLTree(AVLTree tree, Type key) //删除节点
{
    AVLNode *z;
    if ((z = searchAVLTree(tree, key)) != NULL)
    { //先判断有没有找到该节点
        tree = deleteAVLNode(tree, z);
    }
    return tree;
}

void changeAVLTree(AVLTree tree, Type key, Type val) //修改节点 属性
{
    deleteAVLTree(tree, key); //采取 先删后插
    insertAVLTree(tree, val);
}

int GetLevelNums(AVLTree tree, int k) //求第k层的节点个数
{
    if (tree == NULL || k == 0)
        return 0;
    if (k == 1)
        return 1;
    return GetLevelNums(tree->lchild, k - 1) + GetLevelNums(tree->rchild, k - 1);
}

void getPathFromRoot(AVLNode *tree, Type key) //通过key 来获取 路径
{
    Stack stack;
    InitStack(&stack);
    AVLNode *node = searchAVLTree(tree, key);
    if (node == NULL)
    {
        printf("the node is not exist\n");
        return;
    }
    if (GetPath(tree, node, &stack))
    {
        Stack temp;
        InitStack(&temp);
        while (!isStackEmpty(&stack))
        {
            Push(&temp, Pop(&stack));
        }
        while (!isStackEmpty(&temp))
        {
            visit(Pop(&temp));
        }
    }
    else
    {
        printf("the path is not exist\n");
    }
}

bool GetPath(AVLNode *tree, AVLNode *node, pStack stack) //获得根到该节点的路径
{
    if (tree == node)
    {
        Push(stack, tree);
        return true;
    }
    if (tree == NULL)
        return false;
    bool found = false;
    Push(stack, tree);
    found = GetPath(tree->lchild, node, stack);
    if (!found)
        found = GetPath(tree->rchild, node, stack);
    if (!found)
        Pop(stack);
    return found;
}

void printList(AVLNode *node) //二叉树转化为 广义表
{
    visit(node);
    if (node->lchild != NULL)
    {
        printf("(");
        printList(node->lchild);
        if (node->rchild == NULL)
            printf(",#)");
    }
    if (node->rchild != NULL)
    {
        if (node->lchild == NULL)
            printf("(#");
        printf(",");
        printList(node->rchild);
        printf(")");
    }
}

Type getType() //获取输入
{
    char c;
    scanf("%c", &c);
    if (c == '#')
        return -1;
    return c - 48;
}

void createAVLTree(AVLTree tree) //输入char字符 生成 二叉树
{
    Type c = getType();
    if (c == -1)
        tree == NULL;
    else
    {
        tree = (AVLTree)malloc(sizeof(AVLNode));
        tree->key = c;
        createAVLTree(tree->lchild);
        createAVLTree(tree->rchild);
    }
}

void getAllElement(AVLTree tree, pQueue pq) //获得一颗平衡树的所有元素
{
    if (tree == NULL)
        return;
    InitQueue(pq);
    EnQueue(pq, tree);
    if (tree->lchild != NULL)
        getAllElement(tree->lchild, pq);
    if (tree->rchild != NULL)
        getAllElement(tree->rchild, pq);
}

void unionTwoAVLTree(AVLTree tree1, AVLTree tree2) //合并两颗平衡树
{
    Queue queue;
    getAllElement(tree2, &queue);
    while (!isQueueEmpty(&queue))
    {
        tree1 = insertAVLTree(tree1, DeQueue(&queue)->key);
    }
}

bool eq(Type key1, Type key2) //判断两者相等操作
{
    return key1 == key2;
}

void splitAVLTree(AVLTree tree, AVLTree *tree1, AVLTree *tree2, Type key) //分裂树
{                                                                         //小的部分 不用直接 就是平衡的了，  大的部分 取每一部分 分别插入
    AVLTree temp = tree;
    AVLTree temp1 = NULL;
    if ((searchAVLTree(temp, key)) == NULL)
    {
        printf("not find the split key in this tree!\n");
        return;
    }
    temp = tree;
    while ((temp != NULL) && (temp->key != key))
    {
        if (less(key, temp->key))
        {
            if (eq(key, temp->lchild->key))
            {
                temp1 = temp->lchild;
                break;
            }
            temp = temp->lchild;
        }
        else
        {
            if (eq(key, temp->rchild->key))
            {
                temp1 = temp->rchild;
                break;
            }
            temp = temp->rchild;
        }
    }
    visit(temp);
    visit(temp1);
    printf("\n");
    Queue queue;
    InitQueue(&queue);
    if (temp1 == temp->lchild)
    {
        temp->lchild = NULL;
    }
    else
    {
        temp->rchild = NULL;
    }
    *tree1 = temp1;
    getAllElement(temp, &queue);
    *tree2 = NULL;
    while (!isQueueEmpty(&queue))
    {
        *tree2 = insertAVLTree(*tree2, DeQueue(&queue)->key);
    }
}

int main()
{
    AVLTree tree = NULL;
    AVLTree tree1 = NULL;
    AVLTree tree2 = NULL, tree3 = NULL;
    int a[10] = {1, 2, 8, 5, 15, 3, 88, 45, 14, 25};
    int b[5] = {4, 9, 10, 250};
    for (int i = 0; i < 10; i++)
        tree = insertAVLTree(tree, a[i]);
    // for (int i = 0; i < 4; i++)
    //     tree1 = insertAVLTree(tree1, b[i]);
    // inOrderAVLTree(tree);
    // printf("\n");

    // unionTwoAVLTree(tree, tree1);
    // inOrderAVLTree(tree);
    // printf("\n");

    // printList(tree);
    // printf("\n");

    // printf("yes\n");
    // splitAVLTree(tree, &tree2, &tree3, 15);
    // inOrderAVLTree(tree2);
    // printf("\n");
    printAVLTreeByLevel(tree);
    system("pause");
    return 0;
}