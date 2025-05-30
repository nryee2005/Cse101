#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

// Comparison function for integers
int compare_ints(void *a, void *b) {
    int x = *(int *)a;
    int y = *(int *)b;
    return x - y; // Min-heap: lower number = higher priority
}


int main() {
    PriorityQueue pq;
    pq_init(&pq, compare_ints); // initalise and allocating memory for the priority queue.
    //compare_int - is a custom compartor function which passed as an argument to compare int type data and return an integer.

    int *a = malloc(sizeof(int)); *a = 10;
    int *b = malloc(sizeof(int)); *b = 30;
    int *c = malloc(sizeof(int)); *c = 20;
    int *d = malloc(sizeof(int)); *d = 50;
    int *e = malloc(sizeof(int)); *e = 60;

    pq_insert(&pq, a);
    pq_insert(&pq, b);
    pq_insert(&pq, c);
    pq_insert(&pq, d);
    pq_insert(&pq, e);
    print_queue(&pq);

    printf("Priority Queue Output:\n");
    while (!pq_is_empty(&pq)) {
        int *val = (int *)pq_delete(&pq);
        printf("Deleted: %d\n", *val);
        free(val);
        print_queue(&pq);
    }

    pq_destroy(&pq);
    return 0;
}