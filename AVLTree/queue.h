#include "ALL.h"
#ifndef _QUEUE_H_
#define _QUEUE_H_

void InitQueue(pQueue queue);  //初始化 队列 操作
bool isQueueEmpty(pQueue queue);  //判断队列 是否为空
bool EnQueue(pQueue queue, AVLNode* node);  //入队列
AVLNode* DeQueue(pQueue queue);  //出队列

#endif