#include "syntax_tree.h"

int main(void)
{
    struct TreeNode *tree = parse();
    printTree(tree, 0);
}