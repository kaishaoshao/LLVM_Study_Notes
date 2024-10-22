#include "syntax_tree.h"

struct TreeNode *createTreeNode(char *type)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printTree(struct TreeNode *node, int level)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }

    printf("%s\n", node->type);
    printTree(node->left, level + 1);
    printTree(node->right, level + 1);
}