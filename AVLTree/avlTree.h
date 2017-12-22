#include "ALL.h"
#ifndef _AVLTREE_H_
#define _AVLTREE_H_

bool less(Type key1, Type key2)  //当要修改 关键字值时改这里就行
{
    return key1 < key2;
}

void initAVLTree(AVLTree tree);  //初始化 二叉平衡树

AVLNode* avlTreeMinimum(AVLTree tree);   //返回key 最小节点

AVLNode* avlTreeMaximum(AVLTree tree);   //返回key 最大节点

void visit(AVLNode* node);  //访问节点
void visitRoot(AVLNode* node);  //访问根节点 并打印出 具体信息
void visitChild(AVLNode* node, Type key, int direction);
//访问非根节点 并打印出 具体信息

void preOrderAVLTree(AVLTree tree);  //前序遍历
void preNoRecursive(AVLTree tree); //非递归 前序遍历

void inOrderAVLTree(AVLTree tree);   //中序遍历
void inNoRecursive(AVLTree tree); //非递归 中序遍历

void postOrderAVLTree(AVLTree tree);   //后序遍历
void postNoRecursive(AVLTree tree); //非递归 后序遍历

void printAVLTreeByLevel(AVLTree tree);  //层次遍历平衡树

AVLNode* searchAVLTree(AVLTree tree, Type key); //递归查找平衡树
AVLNode* noRecursiveSearch(AVLTree tree, Type key); //非递归查找 平衡树
 
int AVLTreeHeight(AVLTree tree); //返回 平衡树的高度

void printAVLTree(AVLTree tree, Type key, int direction);
//打印出 平衡树 的 信息

void destroyAVLTree(AVLTree tree); //销毁平衡树

AVLNode* leftLeftRotation(AVLTree k2);  //左旋转 即顺时针旋转
AVLNode* rightRightRotation(AVLTree k2); //右旋转 即逆时针旋转

AVLNode* leftRightRotation(AVLTree k2);  //先右旋 左子节点 ， 然后再 左旋转 头节点
AVLNode* rightLeftRotation(AVLTree k2);  //先左旋 右子节点， 然后 再 右旋 头节点

AVLNode* createAVLNode(Type key, AVLNode *lchild, AVLNode* rchild);

AVLNode* insertAVLTree(AVLTree tree, Type key);  //插入节点
AVLNode* deleteAVLNode(AVLTree tree, AVLNode *z);  //删除节点 具体操作
AVLNode* deleteAVLTree(AVLTree tree, Type key);  //删除节点 广义操作
void changeAVLTree(AVLTree tree, Type key, Type val); //修改节点 属性 通过先删后插
//即增删查改 已经完成

int GetLevelNums(AVLTree tree, int k); //求第k层的节点个数

void getPathFromRoot(AVLNode* tree, Type key);  //通过key 来获取 路径
bool GetPath(AVLNode* tree, AVLNode* node, pStack stack); //获得根到该节点的路径

void printList(AVLNode* node); //二叉树转化为 广义表

void createAVLTree(AVLTree tree); //通过输入 字符 来 产生 二叉树
//个人觉得 以广义表输入 没有必要， 就没有去考虑这一个

void getAllElement(AVLTree tree, pQueue pq); //获得一颗平衡树的所有元素 
void unionTwoAVLTree(AVLTree tree1, AVLTree tree2); //合并两颗平衡树

void splitAVLTree(AVLTree tree, AVLTree* tree1, AVLTree* tree2, Type key);  //分裂树
#endif