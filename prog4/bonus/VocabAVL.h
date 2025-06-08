#ifndef VOCAB_AVL_H
#define VOCAB_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Structure representing a node in the AVL tree.
//
typedef struct TokenNode {
    char* token;                  // The string token (e.g., "apple")
    int token_id;                 // Unique ID assigned to this token
    struct TokenNode* left;      // Pointer to left child
    struct TokenNode* right;     // Pointer to right child

    // Extra fields for AVL balancing (used in bonus)
    struct TokenNode* parent;    // Pointer to parent node (for rotation updates)
    int height;                  // Height of the node in the tree
    int balanceFactor;           // Balance factor = height(left) - height(right)
} TokenNode;

/**
 * Creates a new TokenNode with the given token string and token_id.
 * Initializes left/right/parent pointers to NULL, height to 1.
 * 
 * @param token     The token string to store in the node.
 * @param token_id  The unique integer ID assigned to the token.
 * @return          Pointer to the newly created node, or NULL on malloc failure.
 */
TokenNode* createNode(const char* token, int token_id);

/**
 * Performs a right rotation (LL case) on the subtree rooted at y.
 * 
 * @param y         Root of the unbalanced subtree.
 * @return          New root after rotation.
 */
TokenNode* rightRotate(TokenNode* y);

/**
 * Performs a left rotation (RR case) on the subtree rooted at x.
 * 
 * @param x         Root of the unbalanced subtree.
 * @return          New root after rotation.
 */
TokenNode* leftRotate(TokenNode* x);

/**
 * Inserts a token into the AVL tree. If the token already exists, does nothing.
 * Automatically balances the tree if needed after insertion.
 * 
 * @param root      Root of the AVL tree.
 * @param token     Token string to insert.
 * @param token_id  ID to assign if the token is new.
 * @param inserted  Output: 1 if inserted, 0 if token already existed.
 * @return          Updated root of the tree after insertion and balancing.
 */
TokenNode* insertToken(TokenNode* root, const char* token, int token_id, int* inserted);

/**
 * Deletes the given token from the AVL tree.
 * Automatically balances the tree after deletion if needed.
 * 
 * @param root      Root of the AVL tree.
 * @param token     Token to delete.
 * @param deleted   Output: 1 if successfully deleted, 0 if token not found.
 * @return          Updated root of the tree after deletion and balancing.
 */
TokenNode* deleteToken(TokenNode* root, const char* token, int* deleted);

/**
 * Searches for the given token in the AVL tree.
 * If found, sets *token_id_out and *depth_out to the corresponding values.
 * 
 * @param root           Root of the AVL tree.
 * @param token          Token to search for.
 * @param token_id_out   Output: token ID if found.
 * @param depth_out      Output: depth in the tree (root is 0).
 * @return               Pointer to the node if found, NULL if not found.
 */
TokenNode* findToken(TokenNode* root, const char* token, int* token_id_out, int* depth_out);

/**
 * Prints the tokens in the AVL tree in lexicographic (inorder) order.
 * Output format: space-separated tokens, no trailing space, newline at end.
 */
void inorderPrint(TokenNode* root);

/**
 * Prints the tokens in preorder (Root, Left, Right) traversal.
 * Output format: space-separated tokens, no trailing space, newline at end.
 */
void preorderPrint(TokenNode* root);

/**
 * Prints the tokens in postorder (Left, Right, Root) traversal.
 * Output format: space-separated tokens, no trailing space, newline at end.
 */
void postorderPrint(TokenNode* root);

/**
 * Frees all memory allocated for the AVL tree rooted at *root.
 * Sets *root to NULL after freeing.
 * 
 * @param root  Pointer to the root pointer of the tree.
 */
void freeTree(TokenNode** root);

#endif