#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
#include <assert.h>



ListPtr createList( void (*dataPrinter)(void *data) ) {
	ListObj *newList = (ListObj*)malloc(sizeof(ListObj));	// make new list object
	newList->head = NULL;
	newList->length = 0;
	newList->dataPrinter = dataPrinter;

	return newList;		//return the new list object
}

void destroyList(ListPtr *pL) {
	if (!pL || !*pL){	// check if the pointer is NULL
		return;
	}

	NodePtr currentNode = (*pL)->head;
	while(currentNode != NULL) {	// interate through each node until you get NULL
		NodePtr temp = currentNode;
		currentNode = currentNode->next;
		free(temp);
	}

	free(*pL);
	*pL = NULL;
}

int lengthList(ListPtr L) {
	if (L == NULL) {
		return -1;
	}

	return L->length;	// return the length of the list 
}

void printList(ListPtr L) {
	if (L == NULL) {
		return;
	}

	NodePtr currentNode = L->head;

	while(currentNode != NULL) {	// interate through each node
		L->dataPrinter(currentNode->data);		// use dataPrinter function in the ListObj struct 
		currentNode = currentNode->next;
	}
}

void *getList(ListPtr L, int i) {
	if (L == NULL || i > L->length - 1 || i<0) {
		return NULL;
	}

	NodePtr headNode = L->head;

	for(int j = 0; j<i; j++) {		// find the node at the given index 
		headNode = headNode->next;
	}

	return headNode->data;		// return data of node 
}

void *deleteList(ListPtr L, int i) {
	if (L == NULL || i > (L->length)-1) {
		return NULL;
	}
	else if (i==0) { 	// handle if you are deleting the first node 
		NodePtr temp = L->head;
		L->head = L->head->next;
		L->length--;
		return temp->data;
	}

	NodePtr currentNode = L->head;
	for(int j = 0; j < (i-1); j++) {	// go to indexed node 
		currentNode = currentNode->next;
	}

	NodePtr temp = currentNode->next;
	currentNode->next = currentNode->next->next;	// update pointers 
	free(temp);
	L->length--;
	return temp->data;
}

bool appendList(ListPtr L, void *data) {
	NodeObj *newNode = (NodeObj*)malloc(sizeof(NodeObj));		// create new node to append to list
	newNode->data = data;
	newNode->next = NULL;

	if (L == NULL) {
		return false;
	}
	else if (L->head == NULL) {
		L->head = newNode;
		L->length++;
		return true;
	}

	NodePtr currentNode = L->head;
	while(currentNode->next != NULL) {		// go to end of list
		currentNode = currentNode->next;
	}

	currentNode->next = newNode;	// add new node to end of list
	L->length++;
	return true;

}