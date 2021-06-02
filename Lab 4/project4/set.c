#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "set.h"

#define NODE struct node //needed for internal reference
#define LIST struct list
#define SET struct set

struct node
{
    void *data;
    NODE *next;
    NODE *prev; //doubly linked list
};

struct list
{
    NODE *head;
    int count;
    int (*compare)();
};

struct set
{
    int count;
    int length;
    char **elts;
    int (*compare)();
    unsigned (*hash)();
};

//same methods as Proj. 3

//O(1)
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
    SET *sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp->length = maxElts/20; //n/alpha [alpha = 20]
    sp->count = 0;
    sp->elts = malloc(sizeof(void*) * maxElts); //throws warnings becasuse compiler doesn't know about line 51
    assert(sp->elts != NULL);
    sp->hash = hash;
    sp->compare = compare;

    int i;
    for (i = 0; i < sp->length; i++) { //init all the lists
        sp->elts[i] = createList(sp->compare); //give element a linked list to handle duplicates with
    }
    return sp;
}

//O(n)
void destroySet(SET *sp)
{
    assert(sp != NULL);
    int i;
    for (i = 0; i < sp->length; i++) { //need to get rid of the lists within the hash table
        destroyList(sp->elts[i]);
    }
    free(sp->elts); //free the array the
    free(sp); //free the set pointer
}

//O(1)
int numElements(SET *sp)
{
    assert(sp != NULL);
    return sp->count;
}

//O(n)
void addElement(SET *sp, void *elt)
{
    assert(sp != NULL);
    int key = ((*sp->hash)(elt)) % sp->length; //get the key for the given element
    void *found = findItem(sp->elts[key], elt); //o(n)
    if (found == NULL)
    {
        addFirst(sp->elts[key], elt); //add to the front of the list in the given key slot
        sp->count++;
    }
}

//O(n)
void removeElement(SET *sp, void *elt)
{
    assert(sp != NULL);
    int key = ((*sp->hash)(elt)) % sp->length;
    void *found = findItem(sp->elts[key], elt); //O(n)
    if (found != NULL)
    {
        removeItem(sp->elts[key], elt); //remove element from the list [abstract to underyling structure]
        sp->count--;
    }
}

//O(n) [hashing is O(n) but n is small (length of the string)]
void *findElement(SET *sp, void *elt)
{
    assert(sp != NULL);
    void *found;
    int i;
    int key = ((*sp->hash)(elt)) % sp->length;
    found = findItem(sp->elts[key], elt); //O(n) [call ADT func to search underlying list]
    return found; //not found
}

//O(n^3) [we have to handle both the list and the set]
void *getElements(SET *sp)
{
    assert(sp != NULL);
    int i, j, counter = 0; 
    void **ret = (void*)malloc(sizeof(void*) * sp->count); //void pointer to array of pointers needed
    assert(ret != NULL);
    for (i = 0; i < sp->length; i++) { //go through the set [O(n)]
        void** itemsInList = getItems(sp->elts[i]); //gather all the items in the current list, i [O(n)]
        for (j = 0; j < numItems(sp->elts[i]); j++) { //O(n); go through each element in the list we just got
            ret[counter] = itemsInList[j]; //match the list array to the list one using a counter, put the items in
            counter++;
        }
    }
    return ret;
}