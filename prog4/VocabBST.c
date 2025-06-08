#include "VocabBST.h"

// Create a new token node
TokenNode* createNode(const char* token, int token_id) {
    TokenNode *newNode = (TokenNode *)malloc(sizeof(TokenNode));
    newNode->token = strdup(token);
    newNode->token_id = token_id;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insert a token into the BST
TokenNode* insertToken(TokenNode* root, const char* token, int token_id, int* inserted) {
    // Insertion
    if (root == NULL) {
        *inserted = 1;
        return createNode(token, token_id);
    } else {
        // Check if the token already exists
        if (strcmp(token, root->token) < 0) {
            root->left = insertToken(root->left, token, token_id, inserted);
        } else if (strcmp(token, root->token) > 0) {
            root->right = insertToken(root->right, token, token_id, inserted);
        } else {
            *inserted = 0;
        }
        return root;
    }
}

// Find a token with depth reporting
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

// Delete a token
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
            // Replace with inorder successor
            TokenNode* temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            root->token = strdup(temp->token); 
            root->token_id = temp->token_id;
            root->right = deleteToken(root->right, temp->token, deleted);
        }
    }
    return root;
        

}

// Inorder traversal
void inorderPrint(TokenNode* root) {
    if (root == NULL) {
        return;
    } else {
        inorderPrint(root->left);
        printf("%s ", root->token);
        inorderPrint(root->right);
    } 
}

// Preorder traversal
void preorderPrint(TokenNode* root) {
    if (root == NULL) {
        return;
    } else {
        printf("%s ", root->token);
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}

// Postorder traversal
void postorderPrint(TokenNode* root) {
    if (root == NULL) {
        return;
    } else {
        postorderPrint(root->left);
        postorderPrint(root->right);
        printf("%s ", root->token);
    }
}

// Free entire tree
void freeTree(TokenNode** root) {
    if (*root != NULL) {
        freeTree(&(*root)->left);
        freeTree(&(*root)->right);
        free((*root)->token);
        free(*root);
        *root = NULL;
    }
}