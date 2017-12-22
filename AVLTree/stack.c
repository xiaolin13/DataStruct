#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void InitStack(pStack ps)  //初始化栈
{
    ps->num = 0;
    ps = (pStack)malloc(sizeof(Stack));
    if(ps == NULL)
    {
        printf("Dynamically allocated memory failure.\n");
        //动态分配内存失败
        exit(-1);
    }
    ps->Top = (pNode)malloc(sizeof(Node));
    if(ps->Top == NULL)
    {
        printf("Dymnamically allocated memory failure.\n");
        exit(-1);
    }
    ps->Buttom = ps->Top;
    ps->Top->next = NULL; 
}

bool Push(pStack ps, AVLNode* node)  //进栈操作
{
    pNode pNew = (pNode)malloc(sizeof(Node));
    if(NULL == pNew) //因为是链表， 暂时不考虑 栈满的情况
        return false;
    pNew->member = node;
    pNew->next = ps->Top;
    ps->Top = pNew;
    ps->num++;
    return true;
}

bool isStackEmpty(pStack ps)  //判断非空
{
    if(ps->num == 0)
        return true;
    else 
        return false;    
}

AVLNode* Pop(pStack ps)  //出栈操作
{
    pNode pNew = NULL;
    AVLNode* returnVal;
    if(isStackEmpty(ps))  //空的 话 返回 一个NULL 值
        return NULL;

    returnVal = ps->Top->member;
    pNew = ps->Top;
    ps->Top = ps->Top->next;
    free(pNew);  //free 掉那块内存
    ps->num--;
    return returnVal;
}

