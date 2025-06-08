#include "VocabAVL.h"
#include <string.h>

#define MAX_CMD_LEN 128
#define MAX_TOK_LEN 64

int main() {
    TokenNode *root = NULL;
    int token_id = 0;
    // insert tokens
    int size;
    scanf("%d", &size);
    char word[MAX_TOK_LEN];
    for (int i = 0; i < size; i++) {
        int inserted;
        scanf("%s", word);
        root = insertToken(root, word, token_id++, &inserted);
    }
    // process commands
    char input[MAX_CMD_LEN];
    scanf("%s", input);
    while (strcmp(input, "exit") != 0) {

        if (strcmp(input, "insert") == 0) {
            int inserted = -1;
            scanf("%s", word);
            root = insertToken(root, word, token_id++, &inserted);

            if (inserted == 0) {
                printf("Token already exists\n");
            } else if (inserted == 1) {
                printf("Inserted: %s (id=%d)\n", word, token_id - 1);
            }

        } else if (strcmp(input, "delete") == 0) {
            int deleted = 0;
            scanf("%s", word);
            root = deleteToken(root, word, &deleted);

            if (deleted == 0) {
                printf("Token not found\n");
            } else if (deleted == 1) {
                printf("Deleted: %s\n", word);
            }

        } else if (strcmp(input, "find") == 0) {
            int token_id = -1;
            int depth_out = -1;
            scanf("%s", word);
            findToken(root, word, &token_id, &depth_out);

            if (token_id == -1) {
                printf("Not found for the token %s\n", word);
            } else {
                printf("Found: %s (id=%d) (Depth: %d)\n", word, token_id, depth_out);
            }

        } else if (strcmp(input, "inorder") == 0) {
            inorderPrint(root);
            printf("\n");
        } else if (strcmp(input, "preorder") == 0) {
            preorderPrint(root);
            printf("\n");
        } else if (strcmp(input, "postorder") == 0) {
            postorderPrint(root);
            printf("\n");
        }

        scanf("%s", input);
    }

    freeTree(&root);
    return 0;
}