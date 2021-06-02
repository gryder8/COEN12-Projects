#include "pqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct pqueue
{ //typedef from header
    int count;
    int length;
    void **data;
    int (*compare)();
} PQ;

//O(1)
void swap(PQ *pq, int a, int b)
{ //swaps two pieces of data in the array
    //swaps a with b
    // printf("Count: %d \n", pq->count);
    // printf("a in swap: %d \n", a);
    // printf("b in swap: %d \n", b);
    assert(pq != NULL);
    assert(a <= pq->count);
    assert(b <= pq->count); //check bounds
    //printf("Swapping %d with %d \n", pq->data[a], pq->data[b]); //testing on ints so we can be lazy since we know void* is really int
    void *temp = pq->data[a];
    pq->data[a] = pq->data[b];
    pq->data[b] = temp;
}

//O(n)
void reheapUp(PQ *pq, int parentIndex, int childIndex)
{
    while ((*pq->compare)(pq->data[childIndex], pq->data[parentIndex]) == -1) //child < parent
    { //while the child is less than the parent, swap them and update the indexes
        // printf("Parent Index: %d \n", parentIndex);
        //printf("Reheap up \n");
        swap(pq, parentIndex, childIndex); //swap the parent and child if the above condtition is true

        childIndex = parentIndex; //update  to account for the swap
        parentIndex = (parentIndex - 1) / 2; //reassign
    }
}

/* Reheap Down Cases:
    -compare both child to find min (they both exist)
    -only left child exists, is min
    -only right child exists, is min
    -no children
*/

//O(n)
void *reheapDown(PQ *pq, void *root)
{
    assert(pq != NULL);
    int count = pq->count;
    int parent = 0; //front of the arrray
    int minChild = 0;
    int leftChild = (parent * 2 + 1);
    int rightChild = (parent * 2 + 2);
    while (parent < count) {
        /*
        Finding the mind child of the parent
        */
        if (leftChild < count && rightChild < count) { //both children exist
            if ((pq->compare)(pq->data[leftChild], pq->data[rightChild]) == -1) { //left less than right
                minChild = leftChild;
            } else { //less than
                minChild = rightChild;
            }
        } else if (leftChild < count) { //only left child exists; we can ignore the right child due to the nature of heap
            minChild = leftChild;
        } else { //no children exist
            return root;
        }

        /*
        Reheaping Down
        */
        if ((pq->compare)(pq->data[parent], pq->data[minChild]) == 1)
        { //parent is greater than the minChild
            //printf("Min Child: %d \n", minChild);
            //printf("Reheap down \n");

            //swap child and parent
            swap(pq, minChild, parent);

            parent = minChild;
            leftChild = parent * 2 + 1; //reassign
            rightChild = parent * 2 + 2;
        }
        else
        {
            break; //nothing to do
        }
    }
    return root;
}



/*
Parent: (((x) - 1) / 2)
Left Child: ((x) * 2 + 1)
Right Child: ((x) * 2 + 2)
Don't move entry while reheaping until you find where it belongs
*/

//O(1)
PQ *createQueue(int (*compare)())
{
    PQ *pq = malloc(sizeof(PQ));
    assert(pq != NULL);
    pq->count = 0;
    pq->length = 10;
    pq->compare = compare;
    pq->data = malloc(sizeof(void *) * pq->length);
    assert(pq->data != NULL); //make sure data is allocated
    return pq;
}

void destroyQueue(PQ *pq)
{
    assert(pq != NULL);
    free(pq->data);
    free(pq);
}

int numEntries(PQ *pq)
{
    assert(pq != NULL);
    return pq->count;
}

void addEntry(PQ *pq, void *entry)
{ //reheap up when adding!
    assert(pq != NULL);

    pq->data[pq->count] = entry;

    int x = pq->count;
    int child = x; //we're adding a child
    int parent = (x - 1) / 2;

    reheapUp(pq, parent, child);
    pq->count++;
    if (pq->count == pq->length)
    { //more memory needed
        pq->length *= 2;
        pq->data = realloc(pq->data, sizeof(void *) * pq->length); //expensive
        assert(pq->data != NULL);
    }
}

void *removeEntry(PQ *pq)
{ //reheap down when deleting! [refer to lecture notes]
    assert(pq != NULL);
    assert(pq->count > 0);                 //make sure we have things to remove
    void *startRoot = pq->data[0];         //remember root
    pq->data[0] = pq->data[pq->count - 1]; //set the first element to be the last one
    pq->count--;
    return reheapDown(pq, startRoot);
}


