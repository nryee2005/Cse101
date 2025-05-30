#include "BST.h"

// TODO: Create a new BST node
Node* createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// TODO: Insert a value into the BST
Node* insertNode(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    } 

    if (value < root->value) {
        root->left = insertNode(root->left, value);
    } else if (value > root->value) {
        root->right = insertNode(root->right, value);
    } 

    return root;
}

// TODO: Delete a node from BST
Node* deleteNode(Node* root, int value) {
    if (root == NULL) {
        return root;
    }

    if (root->value == value) {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        } else {
            Node* temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
            return root;
        }
    } else if (root->value > value) {
        root->left = deleteNode(root->left, value);
        return root;
    } else {
        root->right = deleteNode(root->right, value);
        return root;
    }
}

// TODO: Search for a value in the BST and print Found (Depth: d) or Not found
void findNode(Node* root, int value) {
    int depth = 0;

    while (root != NULL) {
        if (root->value == value) {
            printf("Found (Depth: %d)\n", depth);
            return;
        } else if (root->value > value) {
            root = root->left;
            depth++;
        } else {
            root = root->right;
            depth++;
        }
    }

    printf("Not found\n");
}

// TODO: Inorder traversal
void inorderTraversal(Node* root) {
    if (root == NULL) {
        return;
    } else {
        inorderTraversal(root->left);
        printf("%d ", root->value);
        inorderTraversal(root->right);
    }
}

// TODO: Preorder traversal
void preorderTraversal(Node* root) {
    if (root == NULL) {
        return;
    } else {
        printf("%d ", root->value);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

// TODO: Postorder traversal
void postorderTraversal(Node* root) {
    if (root == NULL) {
        return;
    } else {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->value);
    }
}

// TODO: Free the entire tree
void freeTree(Node** root) {
    if (*root != NULL) {
        freeTree(&(*root)->left);
        freeTree(&(*root)->right);
        free(*root);
        *root = NULL;
    }
}