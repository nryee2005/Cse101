#include "BST.h"
#include <string.h>

int main() {
    Node *root = NULL;

    int size;
    scanf("%d", &size);
    int value;
    while (scanf("%d", &value) == 1) {
        root = insertNode(root, value);
    }
    
    char input[100];
    scanf("%s", input);
    while (strcmp(input, "exit") != 0) {

        if (strcmp(input, "insert") == 0) {
            scanf("%d", &value);
            root = insertNode(root, value);
        } else if (strcmp(input, "delete") == 0) {
            scanf("%d", &value);
            root = deleteNode(root, value);
        } else if (strcmp(input, "find") == 0) {
            scanf("%d", &value);
            findNode(root, value);
        } else if (strcmp(input, "inorder") == 0) {
            inorderTraversal(root);
            printf("\n");
        } else if (strcmp(input, "preorder") == 0) {
            preorderTraversal(root);
            printf("\n");
        } else if (strcmp(input, "postorder") == 0) {
            postorderTraversal(root);
            printf("\n");
        }

        scanf("%s", input);
    }

}
