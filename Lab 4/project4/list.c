#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

#define NODE struct node
#define LIST struct list

struct node {
    void *data;
    NODE *next;
    NODE *prev; //doubly linked list
};

struct list {
    NODE *head;
    int count;
    int (*compare)();
};

//O(1)
LIST *createList(int (*compare)()) { //take in compare function
    LIST *newList = malloc(sizeof(LIST));
    assert(newList != NULL); //make sure we allocated successfully
    newList->count = 0;
    newList->compare = compare;
    newList->head = malloc(sizeof(NODE));

    assert(newList->head != NULL);
    newList->head->next = newList->head; //circular list
    newList->head->prev = newList->head;
    return newList;
}

//O(n) [while loop]
void destroyList(LIST *lp){
    assert(lp != NULL);
    if (lp->count > 0) {
        NODE *curr = lp->head->next; //assign to node after the dummy node
        NODE *delete;
        while (curr != lp->head) {
            delete = curr;
            curr = curr->next; //preserve the list and move forward
            free(delete); //delete the node
        }
        free(lp);
    }
}

//O(1)
int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->count;
}

//O(1)
void addFirst(LIST *lp, void *item) { //add in front
    assert(lp != NULL);
    NODE *add = malloc(sizeof(NODE));
    assert(add != NULL);
    add->data = item; //assign data
    add->next = lp->head->next; //account for dummy node!
    add->prev = lp->head; //back link to head (before dummy node)
    lp->head->next->prev = add; //set the node before the first real node to be the node we are adding
    lp->head->next = add; //insert as next by attaching it next to the head
    lp->count++;

}

//O(1)
void addLast(LIST *lp, void *item) { //add behind
    assert(lp != NULL);
    NODE *add = malloc(sizeof(NODE));
    assert(add != NULL);
    add->data = item;
    add->next = lp->head; //in front of dummy node
    add->prev = lp->head->prev; //node after the node being added is the current last node in the list
    lp->head->prev->next = add;
    lp->head->prev = add;
    lp->count++;
}

//O(1)
void *removeFirst(LIST *lp) {
    assert(lp != NULL);
    assert(lp->count > 0); //make sure we have items to remove
    NODE *rem = lp->head->prev; //don't unlink the list
    lp->head->prev = rem->prev; //lp.head.prev.prev
    void *remData = rem->data;
    free(rem);
    lp->count--;
    return remData;
}

//O(1)
void *removeLast(LIST *lp) {
    assert(lp != NULL);
    assert(lp->count > 0);
    NODE *rem = lp->head->prev; //don't unlink the list
    lp->head->prev = rem->prev; //lp.head.prev.prev
    void *remData = rem->data;
    free(rem);
    lp->count--;
    return remData;
}

//O(1)
void *getFirst(LIST *lp) {
    assert(lp != NULL);
    assert(lp->count > 0);
    return lp->head->next->data; //account for dummy node
}

//O(1)
void *getLast(LIST *lp) {
    assert(lp != NULL);
    assert(lp->count > 0);
    return lp->head->prev->data; //utilize circular list
}

//O(n)
void removeItem(LIST *lp, void *item) {
    assert(lp != NULL);
    assert(lp->count > 0); //make sure there are items to remove
    NODE *curr = lp->head->next;
    while (curr != lp->head) {
        if ((*lp->compare)(curr->data, item) == 0) {
            curr->next->prev = curr->prev; //point to the node behind current instead 
            curr->prev->next = curr->next; //point to the node in front of current instead
            lp->count--;
            free(curr);
            
            break; //end loop! 
        }
        curr = curr->next; //iterate forward
    }
}

//O(n)
void *findItem(LIST *lp, void *item) { 
    assert(lp != NULL);
    NODE *curr = lp->head->next;
    while (curr != lp->head) { //include the dummy node since we're initializing to it (whoops)
        if ((*lp->compare)(curr->data, item) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL; //nothing found
}


//O(n)
void *getItems(LIST *lp){
    assert(lp != NULL);
    int i = 0;
    void **ret = malloc(sizeof(void*) * lp->count); //array of n pointers
    assert(ret != NULL); //make sure we got the memory
    NODE *tmp = lp->head->next;
    while (i < lp->count && tmp != lp->head) {
        ret[i] = tmp->data;
        tmp = tmp->next; //increment
        i++; //increment
    }

    return ret;
}

