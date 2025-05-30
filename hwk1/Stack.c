#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "List.h"
#include <assert.h>

// Constructor for the Stack ADT
StackPtr createStack(void (*dataPrinter)(void *data)) {
   StackObj *newStack = (StackObj*)malloc(sizeof(StackObj));   // create new stack object
   newStack->top = NULL;
   newStack->dataPrinter = dataPrinter;
   newStack->length = 0;

   return newStack;
}

// Destructor for the Stack ADT
void destroyStack(StackPtr *pS) {
   if (*pS == NULL || pS == NULL) {    // check if stack is empty
      return;
   }

   NodePtr currentNode = (*pS)->top;

   while (currentNode != NULL) {    // interate through each node in stack and free 
      NodePtr temp = currentNode;
      currentNode = currentNode->next;
      free(temp);
   }

   free(*pS);
   *pS = NULL;
}

// Returns the length of the stack
int lengthStack(StackPtr S) {
   if (S == NULL) {
      return -1;
   }

   return S->length;    // return length of stack
}

// Prints the data in the stack
void printStack(StackPtr S) {
   if (S==NULL) {
      return;
   }
   
   NodePtr currentNode = S->top;

   while (currentNode != NULL) {    // iterate through each node in the stack and use the dataPrinter function from StackObj
      S->dataPrinter(currentNode->data);
      currentNode = currentNode->next;
   }
}

// Retrieves the data at the top of the stack without removing it
void *peekStack(StackPtr S) {
   if (S->top == NULL) {      // no data if stack is empty
      return NULL;
   }

   return S->top->data;    // simply returns the data at the top of the stack
}

// Pushes an entry onto the stack.
bool pushStack(StackPtr S, void *data){
   NodeObj *newNode = (NodeObj*)malloc(sizeof(NodeObj));    // create new node to push to the top of the stack
   newNode->data = data;
   newNode->next = S->top;

   S->top = newNode; 
   S->length++;
   return true;
}

// Pops the entry from the top of the stack and returns the data from that entry.
void *popStack(StackPtr S){
   if (S->top == NULL) {
      return NULL;
   }

   NodeObj *topNode = S->top;
   S->top = S->top->next;     // update the top of the stack
   void *data = topNode->data;
   free(topNode);    // free deleted node and return data
   S->length--;
   return data; 
}