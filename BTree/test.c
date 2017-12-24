#include <stdio.h>
#include <stdlib.h>
#include "BTree.c"

int main()
{
    Node *root = initBTree(7, (char*)"100");
    char ch[10][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9","10"};
    int random;
    Result r;
    for(int i = 1; i <= 10; i++)
    {
        random = i;
        putNode(&root, random, ch[i - 1]);
    }

    levelTraverse(root);
    btree_delete(&root, 4);
    levelTraverse(root);
    printf("\n");
    system("pause");
    return 0;
}