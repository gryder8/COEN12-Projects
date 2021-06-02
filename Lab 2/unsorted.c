#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> //needed for strcmp
#include "set.h"

struct set { //defined to SET using typedef in set.h
    char **data;
    int length;
    int count;
};

SET *createSet(int maxElts){ //allocate memory for a set and assign properties
    SET *sp = malloc(sizeof(SET));
    assert(sp!=NULL);
    sp->count = 0;
    sp->length = maxElts;
    sp->data = malloc(sizeof(char*)*maxElts); //allocate memory for the array
    assert(sp->data != NULL);
    return sp;
}

int search(SET *sp, char *findMe) { //private
    assert(sp != NULL);
    int i;

    //Time complexity: O(n)
    for (i = 0; i < sp->count; i++) {
        if (strcmp(sp->data[i],findMe) == 0) { //returns 0 on equality
            return i; //return the index it was found at
        }
    }
    return -1; //not found!
}

char* findElement(SET *sp, char *elt) {
    assert(sp != NULL);
    int indice = search(sp, elt);
    if (indice != -1) {
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
    char *add = strdup(elt); //make a duplicate so we can add without pointing to same address over and over when adding
    int contains = search(sp, elt); 
    if (contains == -1){
        assert(sp->length > sp->count);
        sp->data[sp->count] = add;
        sp->count++;
    }
}

void removeElement(SET *sp, char *elt){
    assert(sp!=NULL);
    int contains = search(sp, elt);
    if (contains != -1){ //element in the set
        free(sp->data[contains]);
        sp->count--;
        sp->data[contains] = sp->data[sp->count]; //reassign the data to the data before it
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




