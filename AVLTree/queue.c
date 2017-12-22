#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

static int numQueue = 0;

void InitQueue(pQueue queue)  //初始化队列操作
{
    queue = (pQueue)malloc(sizeof(Queue));
    if(queue == NULL)
    {
        printf("Dynamically allocated memory failure.\n");
        //动态分配内存失败
        exit(-1);
    }
    queue->tail = (pNode)malloc(sizeof(Node));
    if(queue->head == NULL)
    {
        printf("Dymnamically allocated memory failure.\n");
        exit(-1);
    }
    queue->head = queue->tail;
    queue->head->next = NULL; 
}

bool isQueueEmpty(pQueue queue)  //判断队列是否为空
{
    return numQueue == 0;
}

bool EnQueue(pQueue queue, AVLNode* node)  //入队列操作
{
    pNode q = (pNode)malloc(sizeof(Node));
    q->member = node;
    q->next = NULL;
    if(numQueue == 0)
    {
        queue->head = q;
        queue->tail = q;
    }
    else 
    {
        queue->tail->next = q;
        queue->tail = q;
    }
    numQueue++;
    return true;
}

AVLNode* DeQueue(pQueue queue)  //出队列操作
{
    pNode pNew = NULL;
    AVLNode *returnVal;
    if(isQueueEmpty(queue))
        return NULL;
    returnVal = queue->head->member;
    pNew = queue->head;
    queue->head = queue->head->next;
    free(pNew);
    numQueue--;
    return returnVal;
}