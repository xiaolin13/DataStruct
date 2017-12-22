#include "ALL.h"

#ifndef _STACK_H
#define _STACK_H

void InitStack(pStack); //初始化 栈
bool Push(pStack, AVLNode*);  //入栈操作
bool isStackEmpty(pStack); //遍历栈是否为空的函数
AVLNode* Pop(pStack); //出栈操作

#endif