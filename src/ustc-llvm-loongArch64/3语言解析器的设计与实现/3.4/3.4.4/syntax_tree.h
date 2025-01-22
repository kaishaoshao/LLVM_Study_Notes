#pragma once
#include <stdio.h>
#include <stdlib.h>

struct TreeNode
{
    char *type;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *createTreeNode(char *type);

void printTree(struct TreeNode *node, int level);

struct TreeNode *parse();