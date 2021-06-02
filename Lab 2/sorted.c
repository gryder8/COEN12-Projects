#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h> //needed for strcmp
#include "set.h"

struct set { //defined to SET using typedef in set.h
    char **data;
    int length;
    int count;
};

//static bool foundElement = false;

SET *createSet(int maxElts){ //allocate memory for a set and assign properties
    SET *sp = malloc(sizeof(SET));
    assert(sp!=NULL);
    sp->count = 0;
    sp->length = maxElts;
    sp->data = malloc(sizeof(char*)*maxElts); //allocate memory for the array
    assert(sp->data != NULL);
    return sp;
}

/*
Because this function goes through the set comparing elt to the other strings, if elt is not found then low = the postion where this element *would* 
be if it were to be found in the set. We can return this (using a boolean flag to check if found instead) and use it to add the element more easily. 
*/

//Time Complexity: O(log(n))
int search(SET *sp, char *findMe, bool *found) { //private
    assert(sp != NULL);

    int low = 0;
    int high = sp->count - 1;
    int mid = (low+high) / 2;
    //Time complexity: O(n)
    while (low <= high) {
        mid = (low+high) / 2;
        if (strcmp(sp->data[mid], findMe) == 0) {
            *found = true;
            return mid;
        } else if (strcmp(sp->data[mid], findMe) > 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    *found = false;
    return low; //not found!
}

char* findElement(SET *sp, char *elt) {
    assert(sp != NULL);
    bool didFind;
    int indice = search(sp, elt, &didFind);
    if (didFind) {
        return sp->data[indice];
    }
    return NULL; //not found
}

int numElements(SET *sp){ //return the amount of elements currently in the set
    assert(sp!= NULL); //make sure SP is not null
    return sp->count;
}

void addElement(SET *sp, char *elt){ //add a new element to the set
    assert(sp != NULL);
    bool didFind;
    char *add = strdup(elt); //make a duplicate so we can add without pointing to same address over and over
    int index = search(sp, elt, &didFind); 
    if (!didFind){
        assert(sp->length > sp->count);
        int i;
        for (i = sp->count-1; i > index-1; i--) { //walk backwards
            sp->data[i+1] = sp->data[i]; //shift data up
        }
        sp->data[index] = add;
        sp->count++;
    }
}

void removeElement(SET *sp, char *elt){
    assert(sp!=NULL);
    bool didFind;
    int position = search(sp, elt, &didFind);
    if (didFind) { //element in the set
        free(sp->data[position]);
        int i;
        for (i = 1 + position; i < sp->count; i++) {
            sp->data[i-1] = sp->data[i]; //shift the data in front backwards to account for the removed element 
        }
        sp->count--; //decrement (after the loop)
    }
}


char **getElements(SET *sp) { //return an array of pointers to char arrays (array of strings)
    assert(sp != NULL);
    char **ret = malloc(sizeof(char*)*sp->count); 
    return memcpy(ret, sp->data, sizeof(char*)*sp->count); //copy the array of the correct size n*strings into the ret var and then return it
    //memcpy returns the destination var
}

void destroySet(SET *sp) { //free all the data associated with the set struct
    assert(sp != NULL);
    int i;
    for (i=0; i < sp->count; i++){
        free(sp->data[i]); //free the data stored in the array
    }
    free(sp->data); //free the array memory itself
    free(sp); //free the memory holding the struct
}




