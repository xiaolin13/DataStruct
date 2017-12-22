#include <stdio.h>
#include <stdlib.h>
#include "BTree.c"

int main()
{
    Node *root = initBTree(8, "root");
    char ch[10][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
    int random;
    for(int i = 1; i <= 10; i++)
    {
        random = i + 2;
        putNode(&root, random, ch[i - 1]);
    }
    for(int i = 1; i <= 10; i++)
    {
        random = i + 2;
        printf("%s ", searchNode(root, random));
    }
    printf("\n");
    //printf("\n");
    //levelTraverse(root);
    printf("%s\n", maximunValue(root));
    system("pause");
    return 0;
}