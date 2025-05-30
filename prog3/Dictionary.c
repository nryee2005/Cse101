#include "List.h"
#include "HashTable.h"
#include "Dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dictionary {
    int slots;
    int size;
    ListPtr *hash_table;
} Dictionary;

// Helper function to find a node with a specific key in a list and return its index.
// Returns -1 if the key is not found.
// This fuction is optional, just recommended to reduce duplication of code
static int find_key_index(ListPtr L, char *key) {
    if (!L) {
        return -1;
    }

    int index = 0;
    // Traverse the list to find the key
    while (index < L->length) {
        KVPair *node = (KVPair *)getList(L, index);
        if (strcmp(node->key, key) == 0) {
            return index;
        }
        index++;
    }

    return -1;
}

Dictionary *dictionary_create(int hash_table_size, void (*dataPrinter)(void *data)) {
    Dictionary *newDict = (Dictionary *)malloc(sizeof(Dictionary));
    newDict->hash_table = (ListPtr *)malloc(sizeof(ListPtr) * hash_table_size);
    // Initialize the hash table with empty lists
    for (int i = 0; i < hash_table_size; i++) {
        newDict->hash_table[i] = createList(dataPrinter);
    }
    newDict->slots = hash_table_size;
    newDict->size = 0;

    return newDict;
}

void dictionary_destroy(Dictionary *d) {
    if (!d) {
        return;
    }
    // Destroy each list in the hash table
    for (int i = 0; i < d->slots; i++) {
        destroyList(&(d->hash_table[i]));
    }
    // Free the hash table array and the dictionary itself
    free(d->hash_table);
    free(d);
}

bool dictionary_insert(Dictionary *D, KVPair *elem) {
    if (!D || !elem) {
        return false;
    }
    // Hash the key to get the index
    int index = ht_hash(elem->key, D->slots);
    // If the key already exists, return false
    if (find_key_index(D->hash_table[index], elem->key) != -1) {
        return false;
    }
    // Insert the key-value pair into the hash table
    appendList(D->hash_table[index], elem);
    D->size++;
    return true;
}

KVPair *dictionary_delete(Dictionary *D, char *key) {
    if (!D || !key) {
        return NULL;
    }
    // Hash the key to get the index
    int index = ht_hash(key, D->slots);
    // If the key doesn't exist, return NULL
    if (find_key_index(D->hash_table[index], key) == -1) {
        return NULL;
    }
    // Delete the key-value pair from the hash table
    D->size--;
    return deleteList(D->hash_table[index], find_key_index(D->hash_table[index], key));
}

KVPair *dictionary_find(Dictionary *D, char *k) {
    if (!D || !k) {
        return NULL;
    }
    // Hash the key to get the index
    int index = ht_hash(k, D->slots);
    // If the key doesn't exist, return NULL
    if (find_key_index(D->hash_table[index], k) == -1) {
        return NULL;
    }
    // Find the key-value pair in the hash table
    return (KVPair*)getList(D->hash_table[index], find_key_index(D->hash_table[index], k));
}

void dictionary_print(Dictionary *D) {
    if (!D) {
        return;
    }
    // Print each list in the hash table
    for (int i = 0; i < D->slots; i++) {
        printList(D->hash_table[i]);
    }
}