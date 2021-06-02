#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"


const int NUM_ELEMENTS = 50; //base amount

//NODE struct
typedef struct node {
    int numElements; //length of the array
    int firstIndex; //first index ***in queue***
    int numInserted; //number of elements inserted in the array
    struct node *next; //doubly linked list
    struct node *prev;
    void **data; //data in node
} NODE;

//LIST struct
typedef struct list {
    NODE *head; //head and tail ptr
    NODE *tail;
    int count; //nodes in list
} LIST;

//make a new node  with the given number of elements
//O(1)
NODE *makeNode(int nElems) { //helper
    NODE *newNode = malloc(sizeof(NODE));
    assert(newNode != NULL);

    newNode->data = malloc(sizeof(void *) * nElems);
    assert(newNode->data != NULL);

    newNode->next = NULL;
    newNode->prev = NULL;

    newNode->numElements = nElems;
    newNode->firstIndex = 0;
    newNode->numInserted = 0;

    return newNode;
}

//O(1)
bool isNodeArrayFull(NODE *n) { //helper
    return n->numInserted >= n->numElements;
}

//make a new linked list
//O(1)
LIST *createList(void) {
    LIST *newList = malloc(sizeof(LIST));
    assert(newList != NULL);
    
    newList->head = makeNode(NUM_ELEMENTS); //make the new head
    assert(newList->head != NULL);

    newList->count = 0;
    newList->tail = newList->head; //set tail to be the head
    return newList;
}

//Destroy the list
//O(1)
void destroyList(LIST *lp) {
    assert(lp != NULL);
    NODE *rem;
    while (lp->head != NULL) {
        rem = lp->head; //data to be freed
        lp->head = lp->head->next; //move forward
        free(rem->data);
        free(rem);
    }
    free(lp); //finally, free the list
}

//O(1)
int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->count;
}

//Add the given item to the front of the lst
//O(1)
void addFirst(LIST *lp, void *item) {
    assert(lp != NULL && item != NULL);
    if (isNodeArrayFull(lp->head)) { //check
        NODE *add = makeNode(lp->head->numElements * 2); //twice the array size of the previous node
        add->next = lp->head; //insert at very front (new head)
        lp->head->prev = add;
        lp->head = add;
    }
    int firstOpenIndex = ((lp->head->firstIndex + lp->head->numElements - 1) %
                          lp->head->numElements); //next open slot as per directions (f + n -1) %m
    lp->head->firstIndex = firstOpenIndex; //update
    lp->head->data[firstOpenIndex] = item; //set

    //finally, increment
    lp->count++;
    lp->head->numInserted++;
}

//Add the given item to the back of the list
//O(1)
void addLast(LIST *lp, void *item) {
    assert(lp != NULL && item != NULL);
    if (isNodeArrayFull(lp->tail)) {
        NODE *add = makeNode(lp->tail->numElements * 2); //twice the size of the previous node
        add->prev = lp->tail; //insert at back (new tail)
        lp->tail->next = add;
        lp->tail = add;
    }
    int lastOpenIndex = ((lp->tail->firstIndex + lp->tail->numInserted) % lp->tail->numElements); //f + n % m
    lp->tail->data[lastOpenIndex] = item; //slot right before next open slot (see directions formula)
    lp->tail->numInserted++; //inc
    lp->count++;
}

//Remove the first item from the list
//Remove the first node if it's empty as well
//O(1)
void *removeFirst(LIST *lp) {
    assert(lp != NULL);
    if (lp->head->numInserted == 0) { //empty node
        NODE *rem = lp->head; //data to be removed
        lp->head = lp->head->next; //move head forward
        lp->head->prev = NULL; //no prev
        free(rem->data); //free data
        free(rem); //free node
    }
    int idx = lp->head->firstIndex;
    int newIdx = (idx + 1) %
                 lp->head->numElements; //move the index forward 1, making sure we don't go off the end in the process
    lp->head->firstIndex = newIdx;
    lp->head->numInserted--; //item removed
    lp->count--;
    void *remData = lp->head->data[idx]; //data to be removed
    return remData;
}

//Remove the last item from the list
//Remove the last node if it's empty as well
//O(1)
void *removeLast(LIST *lp) {
    assert(lp != NULL);
    if (lp->tail->numInserted == 0) { //empty node
        NODE *rem = lp->tail;
        lp->tail = lp->tail->prev; //move tail forwards
        lp->tail->next = NULL; //no next
        free(rem->data); //free data
        free(rem); //free node
    }
    int idx = ((lp->tail->firstIndex + lp->tail->numInserted) % lp->tail->numElements); //f + n % m
    lp->count--; //we removed a node
    lp->tail->numInserted--; //removed element
    void *temp = lp->tail->data[idx]; //data at the index
    return temp;
}

//get the item at a specified index
//O(log(n)) due to arrays in nodes cutting down size
void *getItem(LIST *lp, int index) {
    assert(lp != NULL); //bounds and null check
    assert(index >= 0 && index < lp->count);
    NODE *ptr = lp->head; //iterator
    while (index >= ptr->numInserted) {
        index -= ptr->numInserted; //decrement as per directions
        ptr = ptr->next;
    }
    void *remData = ptr->data[(ptr->firstIndex + index) % ptr->numElements]; //f + index % m
    return remData;
}

//set the item at the specified index using the same logic as getItem(), just not returning
//O(log(n)) due to the arrays in nodes cutting down size
void setItem(LIST *lp, int index, void *item) { //very similar to get item
    assert(lp != NULL && item != NULL); //check to make sure both exist
    assert(index >= 0 && index < lp->count); //bounds check!
    NODE *ptr = lp->head; //iterator
    while (index >= ptr->numInserted) { //iterate
        index -= ptr->numInserted; //decrement as per the directions
        ptr = ptr->next;
    }
    ptr->data[(ptr->firstIndex + index) % ptr->numElements] = item; //set the item we found
}

