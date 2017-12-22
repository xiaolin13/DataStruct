#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define rb_parent(r) ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r) ((r)->color == RED)
#define rb_is_black(r) ((r)->color == BLACK)
#define rb_set_black(r)     \
    do                      \
    {                       \
        (r)->color = BLACK; \
    } while (0)
#define rb_set_red(r)     \
    do                    \
    {                     \
        (r)->color = RED; \
    } while (0)
#define rb_set_parent(r, p) \
    do                      \
    {                       \
        (r)->parent = (p);  \
    } while (0)
#define rb_set_color(r, c) \
    do                     \
    {                      \
        (r)->color = (c);  \
    } while (0)

RBRoot *create_rbtree()
{
    RBRoot *root = (RBRoot *)malloc(sizeof(RBRoot));
    root->node = NULL;

    return root;
}

void preorder(RBTree tree)
{
    if (tree != NULL)
    {
        printf("%d ", tree->key);
        preorder(tree->left);
        preorder(tree->right);
    }
}

void preorder_rbtree(RBRoot *root)
{
    if (root)
        preorder(root->node);
}

void inorder(RBTree tree)
{
    if (tree != NULL)
    {
        inorder(tree->left);
        printf("%d ", tree->key);
        inorder(tree->right);
    }
}

void inorder_rbtree(RBRoot *root)
{
    if (root)
        inorder(root->node);
}

void postorder(RBTree tree)
{
    if (tree != NULL)
    {
        postorder(tree->left);
        postorder(tree->right);
        printf("%d ", tree->key);
    }
}

void postorder_rbtree(RBRoot *root)
{
    if (root)
        postorder(root->node);
}

Node* search(RBTree x, Type key)
{
    if (x == NULL || x->key == key)
    {
        return x;
    }

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

int rbtree_search(RBRoot *root, Type key)
{
    if (root)
        return search(root->node, key) ? 0 : -1;
}

Node *iterative_search(RBTree x, Type key)
{
    while ((x != NULL) && (x->key != key))
    {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

int iterative_rbtree_search(RBRoot *root, Type key)
{
    if (root)
        return iterative_search(root->node, key) ? 0 : -1;
}

Node *minimun(RBTree tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}

int rbtree_minimum(RBRoot *root, int *val)
{
    Node *node;
    if (root)
        node = minimun(root->node);
    if (node == NULL)
        return -1;
    *val = node->key;
    return 0;
}

Node *maximum(RBTree tree)
{
    if (tree == NULL)
        return NULL;
    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}

int rbtree_maximum(RBRoot *root, int *val)
{
    Node *node;
    if (root)
        node = maximum(root->node);

    if (node == NULL)
        return -1;
    *val = node->key;
    return 0;
}

Node *rbtree_successor(RBTree x) //找后集结点
{
    if (x->right != NULL)
        return minimun(x->right);
    Node *y = x->parent;
    while ((y != NULL) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

Node *rbtree_predecessor(RBTree x)
{
    if (x->left != NULL)
        return maximum(x->left);
    Node *y = x->parent;
    while ((y != NULL) && (x == y->left))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

void rbtree_left_rotate(RBRoot *root, Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        root->node = y;
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rbtree_right_rotate(RBRoot *root, Node *y)
{
    Node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == NULL)
        root->node = x;
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void rbtree_insert_fixup(RBRoot *root, Node *node)
{
    Node *parent, *gparent;
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);
        if (parent == gparent->left)
        {
            {
                Node *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }
            if (parent->right == node)
            {
                Node *tmp;
                rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        } 
        else
        {
            {
                Node *uncle = gparent->left;
                if(uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }
            if(parent->left == node)
            {
                Node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rb_set_black(parent);
            rb_set_red(gparent);  //有问题吧
            rbtree_left_rotate(root, gparent);
        }
    }
    rb_set_black(root->node);
}

// void rbtree_insert_fixup(RBRoot *root, Node *node)
// {
//     Node *parent, *gparent;

//     // 若“父节点存在，并且父节点的颜色是红色”
//     while ((parent = rb_parent(node)) && rb_is_red(parent))
//     {
//         gparent = rb_parent(parent);

//         //若“父节点”是“祖父节点的左孩子”
//         if (parent == gparent->left)
//         {
//             // Case 1条件：叔叔节点是红色
//             {
//                 Node *uncle = gparent->right;
//                 if (uncle && rb_is_red(uncle))
//                 {
//                     rb_set_black(uncle);
//                     rb_set_black(parent);
//                     rb_set_red(gparent);
//                     node = gparent;
//                     continue;
//                 }
//             }

//             // Case 2条件：叔叔是黑色，且当前节点是右孩子
//             if (parent->right == node)
//             {
//                 Node *tmp;
//                 rbtree_left_rotate(root, parent);
//                 tmp = parent;
//                 parent = node;
//                 node = tmp;
//             }

//             // Case 3条件：叔叔是黑色，且当前节点是左孩子。
//             rb_set_black(parent);
//             rb_set_red(gparent);
//             rbtree_right_rotate(root, gparent);
//         } 
//         else//若“z的父节点”是“z的祖父节点的右孩子”
//         {
//             // Case 1条件：叔叔节点是红色
//             {
//                 Node *uncle = gparent->left;
//                 if (uncle && rb_is_red(uncle))
//                 {
//                     rb_set_black(uncle);
//                     rb_set_black(parent);
//                     rb_set_red(gparent);
//                     node = gparent;
//                     continue;
//                 }
//             }

//             // Case 2条件：叔叔是黑色，且当前节点是左孩子
//             if (parent->left == node)
//             {
//                 Node *tmp;
//                 rbtree_right_rotate(root, parent);
//                 tmp = parent;
//                 parent = node;
//                 node = tmp;
//             }

//             // Case 3条件：叔叔是黑色，且当前节点是右孩子。
//             rb_set_black(parent);
//             rb_set_red(gparent);
//             rbtree_left_rotate(root, gparent);
//         }
//     }

//     // 将根节点设为黑色
//     rb_set_black(root->node);
// }

void rbtree_insert(RBRoot *root, Node *node)
{
    Node *y = NULL;
    Node *x = root->node;

    while(x != NULL)
    {
        y = x;
        if(node->key < x->key)
            x = x->left;
        else 
            x = x->right;
    }
    rb_parent(node) = y;
    if(y != NULL)
    {
        if(node->key < y->key)
            y->left = node;
        else 
            y->right = node;
    }
    else 
    {
        root->node = node;
    }

    node->color = RED;
    rbtree_insert_fixup(root, node);  //修正红黑树
}

Node* create_rbtree_node(Type key, Node *parent, Node* left, Node *right)
{
    Node *p;
    if((p = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    p->key = key;
    p->left = left;
    p->right = right;
    p->parent = parent;
    p->color = BLACK;
    return p;
}

int insert_rbtree(RBRoot *root, Type key)
{
    Node *node;
    if(search(root->node, key) != NULL)
        return -1;
    if((node = create_rbtree_node(key, NULL, NULL, NULL)) == NULL)
        return -1;
    rbtree_insert(root, node);

    return 0;
}

void rbtree_destroy(RBTree tree)
{
    if(tree == NULL)
        return;
    if(tree->left != NULL)
        rbtree_destroy(tree->left);
    if(tree->right != NULL)
        rbtree_destroy(tree->right);
    free(tree);
}

// void rbtree_delete_fixup(RBRoot *root, Node* node, Node* parent)
// {
//     Node *other;
//     while((!node || rb_is_black(node)) && node != root->node)
//     {
//         if(parent->left == node)
//         {
//             other = parent->right;
//             if(rb_is_red(other))
//             {
//                 rb_set_black(other);
//                 rb_set_red(parent);
//                 rbtree_left_rotate(root, parent);
//                 other = parent->right;
//             }

//             if((!other->left || rb_is_black(other->left))
//                 && (!other->right || rb_is_black(other->right)))
//             {
//                 rb_set_red(other);
//                 node = parent;
//                 parent = rb_parent(node);
//             }
//             else 
//             {
//                 if(!other->right || rb_is_black(other->right))
//                 {
//                     rb_set_black(other->left);
//                     rb_set_red(other);
//                     rbtree_right_rotate(root, other);
//                     other = parent->right;
//                 }
//                 rb_set_color(other, rb_color(parent));
//                 rb_set_black(parent);
//                 rb_set_black(other->right);
//                 rbtree_left_rotate(root, parent);
//                 node = root->node;
//                 break;
//             }
//         }
//         else
//         {
//             other = parent->left;
//             if(rb_is_red(other))
//             {
//                 rb_set_black(other);
//                 rb_set_red(other);
//                 rbtree_right_rotate(root, parent);
//                 other = parent->left;
//             }
//             if((!other->left || rb_is_black(other->left)) &&
//                 (!other->right || rb_is_black(other->right)))
//             {
//                 rb_set_red(other);
//                 node = parent;
//                 parent = rb_parent(node);
//             }
//             else
//             {
//                 if(!other->left || rb_is_black(other->left))
//                 {
//                     rb_set_black(other->right);
//                     rb_set_red(other);
//                     rbtree_left_rotate(root, other);
//                     other = parent->left;
//                 }

//                 rb_set_color(other, rb_color(parent));
//                 rb_set_black(parent);
//                 rb_set_black(other->left);
//                 rbtree_right_rotate(root, parent);
//                 node = root->node;
//                 break;
//             }
//         }
//     }
//     if(node)
//         rb_set_black(node);
// }

void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
    Node *other;

    while ((!node || rb_is_black(node)) && node != root->node)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

// void rbtree_delete(RBRoot* root, Node *node)
// {
//     Node *child, *parent;
//     int color;

//     //左右节点都不为空
//     if((node->left != NULL) && (node->right != NULL))
//     {
//         Node *replace = node;
//         replace = replace->right;
//         while(replace->left != NULL)
//             replace = replace->left; //求右子树的最小值

//         if(rb_parent(node)) //不是根节点
//         {
//             if(rb_parent(node)->left == node)
//                 rb_parent(node)->left = replace;
//             else 
//                 rb_parent(node)->right = replace;
//         }
//         else 
//             root->node = replace;

//         child = replace->right;
//         parent = rb_parent(replace);
//         color = rb_color(replace);

//         if(parent == node)
//             parent = replace;
//         else 
//         {
//             if(child)
//                 rb_set_parent(child, parent);
//             parent->left = child;
//             replace->right = node->right;
//             rb_set_parent(node->right, replace);
//         }

//         replace->parent = node->parent;
//         replace->color = node->color;
//         replace->left = node->left;
//         node->left->parent = replace;

//         if(color == BLACK)
//             rbtree_delete_fixup(root, child, parent);
//         free(node);
//         return;
//     }

//     //左孩子和右孩子一个为空
//     if(node->left != NULL)
//         child = node->left;
//     else 
//         child = node->right;

//     parent = node->parent;
//     color = node->color;

//     if(child)
//         child->parent = parent;
    
//     if(parent)
//         if(parent->left == node)
//             parent->left = child;
//         else 
//             parent->right = child;
//     else    
//         root->node = child;
//     if(color == BLACK)
//         rbtree_delete_fixup(root, child, parent);
//     free(node);
// }

void rbtree_delete(RBRoot *root, Node *node)
{
    Node *child, *parent;
    int color;

    // 被删除节点的"左右孩子都不为空"的情况。
    if ( (node->left!=NULL) && (node->right!=NULL) ) 
    {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        Node *replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } 
        else 
            // "node节点"是根节点，更新根节点。
            root->node = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代节点"的颜色
        color = rb_color(replace);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node)
        {
            parent = replace;
        } 
        else
        {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbtree_delete_fixup(root, child, parent);
        free(node);

        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else 
        child = node->right;

    parent = node->parent;
    // 保存"取代节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root->node = child;

    if (color == BLACK)
        rbtree_delete_fixup(root, child, parent);
    free(node);
}

void delete_rbtree(RBRoot *root, Type key)
{
    Node *z, *node;
    if((z = search(root->node, key)) != NULL)
        rbtree_delete(root, z);
}

void destroy_rbtree(RBRoot *root)
{
    if(root != NULL)
        rbtree_destroy(root->node);
    free(root);
}

void rbtree_print(RBTree tree, Type key, int direction)
{
    if(tree != NULL)
    {
        if(direction==0)    // tree是根节点
            printf("%2d(B) is root\n", tree->key);
        else                // tree是分支节点
           printf("%2d(%s) is %2d's %6s child\n", tree->key, rb_is_red(tree)?"R":"B", key, direction==1?"right" : "left");
    
        rbtree_print(tree->left, tree->key, -1);
        rbtree_print(tree->right,tree->key,  1);
    }
}

void print_rbtree(RBRoot *root)
{
    if(root != NULL && root->node != NULL)
        rbtree_print(root->node, root->node->key, 0);
}