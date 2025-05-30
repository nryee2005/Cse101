#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"

#define MAX_LINE_LEN 1024 // Maximum length of a line read from file or stdin
#define MAX_TOKEN_LEN 64  // Maximum length of a token (word or subword)
#define MAX_TOKENS 256    // Maximum number of tokens per sentence
#define MAX_ITER 10       // Maximum number of BPE merge iterations

// Structure to store a sentence 
// (actually one word, stored as a token sequence, e.g. corpus[0]: â€œlowerâ€ â†’ ['l', 'o', 'w', 'e', 'r', '</w>']) as a sequence of tokens
typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int token_count;
} Sentence;

// Global corpus and dictionary structures
Sentence corpus[1000];
int corpus_size = 0;

// Global dictionaries:
// - token_to_id: maps token (string) â†’ token ID (string)
Dictionary *token_to_id;
int next_token_id = 0;  // Counter to assign unique token IDs

/**
 * @brief Prints a key-value pair in the format "key: value".
 *
 * @param data Pointer to the KVPair to print.
 */
void print_KVPair(void *data) {
    printf("%s: %s\n", ((KVPair *)data)->key, (char*)((KVPair *)data)->value);
}

/**
 * @brief Adds a new token to the token-to-id and id-to-token dictionaries.
 *        Does nothing if the token already exists.
 *
 * @param token The token string to add.
 */
void add_token(char *token) {
    // Check if the token is already present
    if (dictionary_find(token_to_id, token) != NULL)
        return;

    // Convert the current ID counter to string
    char *id_str = malloc(16);
    sprintf(id_str, "%d", next_token_id);

    // Insert token â†’ ID mapping
    char *token_copy = strdup(token);
    KVPair *fwd = malloc(sizeof(KVPair));
    fwd->key = strdup(token_copy);
    fwd->value = strdup(id_str);
    dictionary_insert(token_to_id, fwd);

    next_token_id++;  // Increment the next available ID
}

/**
 * 
 * @brief Finds the most frequent adjacent token pair in the corpus.
 * 
 * Step-by-step:
 * 1. For each sentence in the corpus:
 *    a. For each adjacent token pair:
 *       - Count how often this pair appears in the entire corpus.
 * 2. Track the pair with the highest frequency.
 * 3. Store the best pairâ€™s left and right tokens in the provided output buffers.
 * 4. Return the highest count found.
 * 5. Called by bpe_train() to determine which pair to merge.
 *
 * @param corpus Array of Sentence structs.
 * @param corpus_size Number of sentences in the corpus.
 * @param best_left Output: the left token of the most frequent pair.
 * @param best_right Output: the right token of the most frequent pair.
 * @return int The frequency count of the most frequent pair.
 */
int find_best_pair(Sentence corpus[], int corpus_size, char* best_left, char* best_right) {
    // Initialize the pairs dictionary
    Dictionary *pairs = dictionary_create(20, print_KVPair);
    int max_count = 0;

    // Count the frequency of each adjacent token pair
    for (int i = 0; i < corpus_size; i++) {
        Sentence *word = &(corpus[i]);
        for (int j = 0; j < word->token_count - 1; j++) {
            if (word->tokens[j] == NULL || word->tokens[j+1] == NULL) {
                continue;  // Check for null pointers
            }

            // Concatenate the left and right tokens
            char *cat = strcat(strdup(word->tokens[j]), strdup(word->tokens[j+1]));
            KVPair *token = dictionary_find(pairs, cat);

            // Update the frequency count
            if (token != NULL) {
                int count = atoi(token->value);
                count++;

                // Update the best pair
                if (count > max_count) {
                    max_count = count;
                    strcpy(best_left, word->tokens[j]);
                    strcpy(best_right, word->tokens[j+1]);
                }

                // Update the dictionary
                char *count_str = malloc(16);
                sprintf(count_str, "%d", count);
                token->value = count_str;
            }
            // Insert the new token
            else {
                KVPair *newToken = (KVPair *)malloc(sizeof(KVPair));
                if (newToken == NULL) {
                    continue;  // Check for malloc failure
                }
                newToken->key = strdup(cat);
                newToken->value = (void *)strdup("1");
                dictionary_insert(pairs, newToken);

                // Update the best pair
                if (max_count < 1) {
                    max_count = 1;
                    strcpy(best_left, word->tokens[j]);
                    strcpy(best_right, word->tokens[j+1]);
                }

            }
        }
    }

    return max_count;
}

/**
 * @brief Merges the specified token pair across the entire corpus.
 * 
 * Step-by-step:
 * 1. Create a merged token by concatenating best_left + best_right.
 * 2. For each sentence in the corpus:
 *    a. Scan tokens left-to-right.
 *    b. When a matching pair is found:
 *       - Replace left token with merged token.
 *       - Shift all following tokens left by one.
 *       - Decrement the sentenceâ€™s token count.
 * 3. Called by bpe_train() after finding the best pair.
 *
 * @param corpus Array of Sentence structs.
 * @param corpus_size Number of sentences in the corpus.
 * @param best_left The left token of the pair.
 * @param best_right The right token of the pair.
 */
void merge_pair(Sentence corpus[], int corpus_size, char* best_left, char* best_right) {
    // Concatenate the left and right tokens
    char *merged = strcat(strdup(best_left), strdup(best_right));

    // Merge the pair in the corpus
    for (int i = 0; i < corpus_size; i++) {
        Sentence *word = &(corpus[i]);
        for (int j = 0; j < word->token_count - 1; j++) {
            // Check if the current token matches the pair
            if (strcmp(word->tokens[j], best_left) == 0 && strcmp(word->tokens[j + 1], best_right) == 0){
                strcpy(word->tokens[j], merged);
                // Shift all following tokens left
                for (int k = j + 1; k < word->token_count - 1; k++) {
                    strcpy(word->tokens[k], word->tokens[k + 1]);
                }
                word->token_count--;
            }
        }
    }

    
}

/**
 * @brief Runs the full BPE training loop.
 * 
 * Step-by-step:
 * 1. Repeat up to max_iter times:
 *    a. Call find_best_pair() to identify the most frequent pair.
 *    b. If no pair is found, stop early.
 *    c. Call merge_pair() to merge the pair in the corpus.
 * 2. Print progress after each iteration. e.g. printf("Iteration %d: merging '%s' + '%s'\n", iter + 1, best_left, best_right);
 *
 * @param corpus Array of Sentence structs.
 * @param corpus_size Number of sentences in the corpus.
 * @param max_iter Maximum number of merge iterations.
 */
void bpe_train(Sentence corpus[], int corpus_size, int max_iter) {
    // Repeat up to max_iter times
    for (int i = 0; i < max_iter; i++) {
        char best_left[MAX_TOKEN_LEN] = "";
        char best_right[MAX_TOKEN_LEN] = "";
        
        // Find the best pair
        find_best_pair(corpus, corpus_size, best_left, best_right);
        // If no pair is found, stop
        if (strcmp(best_left, "") == 0 || strcmp(best_right, "") == 0) {
            break;
        }
        // Merge the pair
        merge_pair(corpus, corpus_size, best_left, best_right);

        printf("Iteration %d: merging '%s' + '%s'\n", i + 1, best_left, best_right);
    }
}

/**
 * @brief Tokenizes an input sentence (multi-word sentence) using the trained BPE vocabulary.
 * 
 * Step-by-step:
 * 1. For each word in the input:
 *    a. Append </w> to mark end of word.
 *    b. Scan from left to right:
 *       - Greedily find the longest matching token using dictionary_find().
 *       - If no match, treat as unknown character and fallback for unknown single character. e.g. printf("[UNK(%s)] ", fallback);
 *    c. Print the matched token and its ID (or UNK). e.g. printf("Word '%s': ", word); printf("[%s -> %s] ", matched_token, (char*)matched_kv->value);
 *
 * @param input Input sentence string.
 * @param token_to_id Dictionary mapping tokens to IDs.
 */
void greedy_bpe_tokenize(char *input, Dictionary *token_to_id) {
    // Split input into words
    char *word = strtok(input, " ");
    while (word) {  
        printf("Word '%s': ", word);
        // Append </w> to mark end of word
        char *word_copy = strdup(word);
        strcat(word_copy, "</w>");

        char *matched_token = malloc(MAX_TOKEN_LEN);
        KVPair *matched_kv = dictionary_find(token_to_id, word_copy);
        // If whole word matches, print it
        if (matched_kv) {
            matched_token = matched_kv->key;
            printf("[%s -> %s]\n", matched_token, (char*)matched_kv->value);
            word = strtok(NULL, " ");
            continue;
        }
        else {
            int start = 0; 
            while (start < strlen(word_copy)) {
                int found = 0;
                // Decrement from the end of the word
                for (int i = strlen(word_copy); i > start; i--) {
                    char *temp = malloc(MAX_TOKEN_LEN);
                    strncpy(temp, word_copy+start, i - start);
                    
                    // If the token matches, print it and update start 
                    if (dictionary_find(token_to_id, temp)) {
                        matched_token = temp;
                        matched_kv = dictionary_find(token_to_id, temp);
                        printf("[%s -> %s] ", matched_token, (char*)matched_kv->value);   
                        start += i - start;
                        found = 1;
                        break;
                    }
                    
                }
                
                // If no match found, treat as unknown character and increment start
                if (found == 0) {
                    printf("[UNK(%c)] ", word_copy[start]);
                    start++;
                }
            }
        }

        // Go to next word
        word = strtok(NULL, " ");
        printf("\n");
    }
    
}

/**
 * @brief Main entry point: reads corpus, trains BPE, builds vocabulary, and processes input.
 *
 * @param argc Argument count.
 * @param argv Argument vector (expects corpus filename as first argument).
 * @return int Exit code.
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <corpus_file>\n", argv[0]);
        return 1;
    }

    // Open corpus file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    // Initialize dictionaries
    token_to_id = dictionary_create(101, print_KVPair);

    char line[MAX_LINE_LEN];

    // Step 1: Read corpus and split into character-level tokens
    while (fgets(line, sizeof(line), fp)) {
        char *word = strtok(line, " \n");
        while (word) {
            Sentence *s = &corpus[corpus_size++];
            s->token_count = 0;

            // Break word into individual characters
            for (int i = 0; i < strlen(word); i++) {
                char ch[2] = {word[i], '\0'};
                strcpy(s->tokens[s->token_count++], ch);
            }

            // Add end-of-word marker
            strcpy(s->tokens[s->token_count++], "</w>");
            word = strtok(NULL, " \n");
        }
    }
    fclose(fp);

    // Step 2: Run BPE merge training
    bpe_train(corpus, corpus_size, MAX_ITER);

    // Step 3: Build final vocabulary from unique tokens
    for (int i = 0; i < corpus_size; i++) {
        for (int j = 0; j < corpus[i].token_count; j++) {
            add_token(corpus[i].tokens[j]);
        }
    }

    printf("\nVocabulary:\n");
    dictionary_print(token_to_id);

    // Step 4: Process user input for BPE tokenization
    printf("\nEnter sentence to tokenize (or Ctrl+D to exit):\n");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;  // Remove newline character
        greedy_bpe_tokenize(line, token_to_id);
    }

    // Clean up
    dictionary_destroy(token_to_id);

    return 0;
}
