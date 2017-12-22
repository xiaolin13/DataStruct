#include <stdio.h>
#include "ALL.h"

int CompareKey(Key key1, Key key2);  //比较两个索引值

Node* initBTree(Key key, String value);  //初始化B树

bool putNode(Node **node, Key key, String value);  //插入节点
Node* put_node(Node **node ,Key key, String value, int depth);  //根据当前节点的状态选择插入方法
Node* put_two(Node **node ,Key key, String value, int depth);  //找到了2-系欸按  然后查找子链接插入
Node* put_three(Node **node, Key key, String value, int depth);  //找到了3-节点  然后查找子链接插入
Node* deal_links(Node **child, Node **now, Key key, String value, int depth); 
void addTwoNode(Node **now, Key key, String value);  //将节点添加到 2-节点
void addThreeNode(Node **now, Key key, String value); //将节点添加到 3-节点
void dealTopTree(Node **root); //处理头节点
Node* dealFourLinks(Node **child, Node **now); // 处理4-节点 只允许暂时存在

String searchNode(Node *node, int key);  //查找
String searchTwoNode(Node *node, int key);
String searchThreeNode(Node *node, int key);

void visit(Node *now);  //访问节点
void levelTraverse(Node *now);  //层次遍历B树

String minimunValue(Node *root);  //查找最小索引对应的值
String maximunValue(Node *root); // 查找最大索引对应的值



