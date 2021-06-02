#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"


//Runtime: O(n)
unsigned strhash(char *s)
{
    unsigned hash = 0;
    while (*s != '\0')
        hash = 31 * hash + *s++;
    return hash;
}

struct set
{
    int count;
    int length;
    char **elts;
    char *flags;
    int (*compare)();
    unsigned (*hash)();
};

#define EMPTY 0
#define FILLED 1
#define REMOVED -1


//Runtime: O(n) [worst case]
static int search(SET *sp, void *elt, bool *found) { //change char* to void*
    assert(sp != NULL);
    bool foundEmpty; //use for conditionally setting firstEmpty
    int key;
    int i;
    int firstEmpty; //return this when nothing is found

    for (i = 0; i < sp->length; i++) {
        key = ((*sp->hash)(elt) + i) % sp->length; //linear probe locally, keep in range using mod
        if (sp->flags[key] == EMPTY) { //empty slot
            *found = false; //not found
            if (!foundEmpty) {
                firstEmpty = key;
                foundEmpty = true;
            }
            break; //end loop!
        } else if (sp->flags[key] == FILLED) {
            if (((sp->compare)(sp->elts[key],elt)) == 0) { //use generic compare
                *found = true;
                return key;
            }
        } else {
            if (!foundEmpty) {
                firstEmpty = key;
                foundEmpty = true;
            }
        }
    }
    *found = false; //we didn't find it if we get here
    return firstEmpty; //return the first empty slot we find
}

//Runtime: O(n) [due to flags]
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
    SET *sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp->length = maxElts;
    sp->count = 0;
    sp->elts = malloc(sizeof(void*) * maxElts);
    assert(sp->elts != NULL);
    sp->flags = malloc(sizeof(int) * maxElts);
    assert(sp->flags != NULL);
    sp->hash = hash;
    sp->compare = compare;

    int i;
    //init all flags to empty
    for (i = 0; i < maxElts; i++) {
        sp->flags[i] = EMPTY;
    }

    return sp;
}

//O(n)
void destroySet(SET *sp) {
    assert(sp != NULL);
    free(sp->elts);
    free(sp->flags);
    free(sp);
}

//O(1)
int numElements(SET *sp) {
    assert(sp != NULL);
    return sp->count;
}

//Runtime: O(n) [search is O(n)]
void addElement(SET *sp, void *elt) {
    assert(sp != NULL);
    bool found;
    int pos = search(sp, elt, &found);
    if (!found) {
        assert(sp->count < sp->length);
        sp->elts[pos] = elt;
        sp->count += 1;
        sp->flags[pos] = FILLED; //set flag
    }
}

//O(n)
void removeElement(SET *sp, void *elt) {
    assert(sp != NULL);
    bool found;
    int pos = search(sp, elt, &found);
    if (found) {
        sp->count -= 1;
        sp->flags[pos] = REMOVED; //set flag
    }
}

//O(n)
void *findElement(SET *sp, void *elt) {
    assert(sp != NULL);
    bool found;
    int pos = search(sp, elt, &found);
    if (found) {
        return sp->elts[pos];
    }
    return NULL; //not found
}

//O(n)
void *getElements(SET *sp) {
    assert(sp != NULL);
    int i;
    int j = 0;
    void** ret = malloc(sizeof(void*) * sp->count); //only holds non-empty elements from the set
    for (i = 0; i < sp->length; i++){
        if (sp->flags[i] == FILLED) {
            ret[j] = sp->elts[i];
            j++;
        }
    }
    return ret;
}
