#include "VocabAVL.h"

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int height(TokenNode* node) {
    if (!node) {
        return -1;
    }
    return node->height;
}

TokenNode* createNode(const char* token, int token_id) {
    TokenNode *newNode = (TokenNode *)malloc(sizeof(TokenNode));

    newNode->token = strdup(token);
    newNode->token_id = token_id;
    newNode->left = NULL;
    newNode->right = NULL;

    newNode->parent = NULL;
    newNode->height = 0;
    newNode->balanceFactor = 0;

    return newNode;
}

// Rotation helpers
TokenNode* rightRotate(TokenNode* y) {
    TokenNode *x = y->left;     // y's left child
    TokenNode *z = x->right;    // y's left child's right child

    x->right = y;   // y's left child's right child = y
    y->left = z;    // y's left child = y's left child's right child
    x->parent = y->parent;     // y's left child's parent = y's parent
    y->parent = x;  // y's parent = y's left child

    if (z) {
        z->parent = y;  // y's left child's right child's parent = y
    }

    // Update heights and balance factors
    y->height = 1 + max(height(y->left), height(y->right));
    y->balanceFactor = height(y->left) - height(y->right);
    x->height = 1 + max(height(x->left), height(x->right));
    x->balanceFactor = height(x->left) - height(x->right);

    return x;
}

TokenNode* leftRotate(TokenNode* x) {
    TokenNode *y = x->right;    // x's right child
    TokenNode *z = y->left;     //  x's right child's left child

    y->left = x;    // x's right child's left child = x
    x->right = z;   // x's right child = x's right child's left child
    y->parent = x->parent;  // x's right child's parent = x's parent
    x->parent = y;  // x's parent = x's right child

    if (z) {
        z->parent = x;  // x's right child's left child's parent = x
    }

    // Update heights and balance factors
    x->height = 1 + max(height(x->left), height(x->right));
    x->balanceFactor = height(x->left) - height(x->right);
    y->height = 1 + max(height(y->left), height(y->right));
    y->balanceFactor = height(y->left) - height(y->right);

    return y;
}

// Insertion
TokenNode* insertToken(TokenNode* node, const char* token, int token_id, int* inserted) {
    if (node == NULL) {
        *inserted = 1;
        return createNode(token, token_id);
    }

    if (strcmp(token, node->token) < 0) {
        node->left = insertToken(node->left, token, token_id, inserted);
    } else if (strcmp(token, node->token) > 0) {
        node->right = insertToken(node->right, token, token_id, inserted);
    } else {
        *inserted = 0;
        return node;
    }

    // Update node's height and balance factor
    node->height = 1 + max(height(node->right), height(node->left));
    node->balanceFactor = height(node->left) - height(node->right);

    // Rebalance if needed
    if (node->balanceFactor < -1 && strcmp(token, node->right->token) > 0) {
        node = leftRotate(node);
    } else if (node->balanceFactor > 1 && strcmp(token, node->left->token) < 0) {
        node = rightRotate(node);
    } else if (node->balanceFactor < -1 && strcmp(token, node->right->token) < 0) {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    } else if (node->balanceFactor > 1 && strcmp(token, node->left->token) > 0) {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    } 

    return node;
}

// Deletion
TokenNode* deleteToken(TokenNode* root, const char* token, int* deleted) {
    if (root == NULL) {
        *deleted = 0;
        return NULL;
    } 

    if (strcmp(token, root->token) < 0) {
        root->left = deleteToken(root->left, token, deleted);
    } else if (strcmp(token, root->token) > 0) {
        root->right = deleteToken(root->right, token, deleted);
    } else {
        // Deletion cases
        if (root->left == NULL && root->right == NULL) {
            free(root->token);
            free(root);
            *deleted = 1;
            return NULL;
        } else if (root->left == NULL) {
            TokenNode* temp = root->right;
            free(root->token);
            free(root);
            *deleted = 1;
            return temp;
        } else if (root->right == NULL) {
            TokenNode* temp = root->left;
            free(root->token);
            free(root);
            *deleted = 1;
            return temp;
        } else {
            // Find the in-order successor
            TokenNode* temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            root->token = strdup(temp->token); 
            root->token_id = temp->token_id;
            root->right = deleteToken(root->right, temp->token, deleted);
        }
    }

    // Update node's height and balance factor
    root->height = 1 + max(height(root->right), height(root->left));
    root->balanceFactor = height(root->left) - height(root->right);

    // Rebalance if needed
    if (root->balanceFactor < -1 && strcmp(token, root->right->token) > 0) {
        root = leftRotate(root);
    } else if (root->balanceFactor > 1 && strcmp(token, root->left->token) < 0) {
        root = rightRotate(root);
    } else if (root->balanceFactor < -1 && strcmp(token, root->right->token) < 0) {
        root->right = rightRotate(root->right);
        root = leftRotate(root);
    } else if (root->balanceFactor > 1 && strcmp(token, root->left->token) > 0) {
        root->left = leftRotate(root->left);
        root = rightRotate(root);
    }

    return root;
}

// Find with depth tracking
TokenNode* findToken(TokenNode* root, const char* token, int* token_id_out, int* depth_out) {
    int depth = 0;
    // Search until the token is found and increment depth
    while (root != NULL) {
        if (strcmp(token, root->token) < 0) {
            root = root->left;
            depth++;
        } else if (strcmp(token, root->token) > 0) {
            root = root->right;
            depth++;
        } else {
            *token_id_out = root->token_id;
            *depth_out = depth;
            return root;
        }
    }
    return NULL;
}

// Traversals
void inorderPrint(TokenNode* root) {
    if (root == NULL) {
        return;
    } else {
        inorderPrint(root->left);
        printf("%s ", root->token);
        inorderPrint(root->right);
    } 
}

void preorderPrint(TokenNode* root) {
    if (root == NULL) {
        return;
    } else {
        printf("%s ", root->token);
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}

void postorderPrint(TokenNode* root) {
    if (root == NULL) {
        return;
    } else {
        postorderPrint(root->left);
        postorderPrint(root->right);
        printf("%s ", root->token);
    }
}

// Free memory
void freeTree(TokenNode** root) {
    if (*root != NULL) {
        freeTree(&(*root)->left);
        freeTree(&(*root)->right);
        free((*root)->token);
        free(*root);
        *root = NULL;
    }
}