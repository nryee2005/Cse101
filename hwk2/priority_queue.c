    // priority_queue.c
    #include <stdlib.h>
    #include <stdio.h>
    #include "priority_queue.h"

    #define INITIAL_CAPACITY 16 

    int parent(int i) {return (i-1) / 2;} //write the return;
    int left(int i) {return (2*i) + 1;}
    int right(int i) {return (2*i) + 2;}

    void swap(void **a, void **b) {
        // make temp variable to hold pointer to a, then swap with b
        void *temp = *a;
        *a = *b;
        *b = temp;
    }

    void heapify_up(PriorityQueue *pq, int idx) {
        // swap data at inx with data of parent if priority of inx lower than parent, go until index is 0
        while (idx > 0 && pq->cmp(pq->data[idx], pq->data[parent(idx)]) < 0) {
            swap(&(pq->data[idx]), &(pq->data[parent(idx)]));
            idx = parent(idx);
        }
    }

    void heapify_down(PriorityQueue *pq, int idx) {
        int current = idx;
        int l = left(idx);
        int r = right(idx); 
        
        // compare current with left child
        if (l < pq->size && pq->cmp(pq->data[l], pq->data[current]) < 0) {
            current = l;
        }
        // compare current with right child
        if (r < pq->size && pq->cmp(pq->data[r], pq->data[current]) < 0) {
            current = r; 
        }

        // only swap if current has moved, if not then idx has the highest priority
        if (current!=idx) {
            swap(&(pq->data[idx]), &(pq->data[current]));
            heapify_down(pq, current);
        }
    }

    void pq_init(PriorityQueue *pq, Comparator cmp) {
        pq->capacity = INITIAL_CAPACITY;
        // allocate void pointer for each index in data array
        pq->data = (void **)malloc(pq->capacity * sizeof(void *));
        if (pq->data == NULL) {
            printf("Error: Memory allocation failed in pq_init\n");
            return;
        }
        // initial size of priority queue is zero
        pq->size = 0;
        pq->cmp = cmp; 
    }

    void pq_insert(PriorityQueue *pq, void *node) {
        // if the pq capacity is full, reallocate the memory to allow for more nodes
        if (pq->size >= pq->capacity) {
            pq->capacity = pq->capacity * 2;
            void **new = realloc(pq->data, sizeof(void *) * pq->capacity);
            if (new == NULL) {
                printf("Error: Memory allocation failed in pq_insert\n");
                return;
            }
            pq->data = new;
        }
        
        // insert node at end of queue, heapify up to keep heap order, increment size
        pq->data[pq->size] = node;
        heapify_up(pq, pq->size);
        pq->size++;
    }

    void *pq_delete(PriorityQueue *pq) {
        if (pq->size == 0) {
            return NULL;
        }

        // swap 1st element with the last, then heapify down on 1st element to keep heap order
        void *data = pq->data[0];
        pq->data[0] = pq->data[pq->size-1];
        // increment size down 1 to exclude deleted node
        pq->size--;
        heapify_down(pq, 0);

        return data;
    }


    void print_queue(PriorityQueue *pq) {
        // print each node up until the size of the queue, not capacity
        for (int i = 0; i < pq->size; i++) {
            printf("%d ", *(int *)pq->data[i]);
        }
        printf("\n");
    }

    int pq_is_empty(PriorityQueue *pq) {
        return pq->size == 0;
    }

    void pq_destroy(PriorityQueue *pq) {
        // free each node
        for (int i = 0; i < pq->size; i++) {
            free(pq->data[i]);
        }

        // free array pointer itself
        free(pq->data);
    }
