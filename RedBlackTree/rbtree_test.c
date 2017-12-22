#include "rbtree.h"
#include "rbtree.c"

#define LENGTH(a) ((sizeof(a)) / sizeof(a[0]))

int main() 
{
    int a[] = {10, 40, 30, 60, 90, 70, 20, 50, 80};
    int i, ilen = LENGTH(a);
    RBRoot *root = NULL;

    root = create_rbtree();
    printf("==the origin data: ");
    for(i = 0; i < ilen; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    for(i = 0; i < ilen; i++)
    {
        insert_rbtree(root, a[i]);
        printf("==add node: %d\n", a[i]);
        printf("==the information of tree: \n");
        print_rbtree(root);
        printf("\n");
    }

    printf("==preorder: ");
    preorder_rbtree(root);
    printf("\n");

    printf("==inorder: ");
    inorder_rbtree(root);
    printf("\n");

    printf("==postorder: ");
    postorder_rbtree(root);
    printf("\n");

    if(rbtree_minimum(root, &i) == 0)
        printf("==the minest: %d\n", i);
    if(rbtree_maximum(root, &i) == 0)
        printf("==the maxest: %d\n", i);
    

    for(i = 0; i < ilen; i++)
    {
        delete_rbtree(root, a[i]);
        printf("==delete root: %d\n", a[i]);
        if(root)
        {
            printf("== the information of tree: \n");
            print_rbtree(root);
            printf("\n");
            printf("==inorder: ");
            inorder_rbtree(root);
            printf("\n");
        }
    }
    getchar();
    return 0;
}