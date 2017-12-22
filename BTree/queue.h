#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "ALL.h"

void InitQueue(pQueue queue);  //初始化 队列 操作
bool isQueueEmpty(pQueue queue);  //判断队列 是否为空
bool EnQueue(pQueue queue, Node* node);  //入队列
Node* DeQueue(pQueue queue);  //出队列

#endif