#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> //for isprint()
#include <assert.h>
#include "pqueue.h" //PQ
#include "pack.h"

typedef struct node NODE;

//O(1)
NODE *makeNode(int count, NODE *left, NODE *right) {
    NODE *new = malloc(sizeof(NODE));
    assert(new != NULL);
    new->count = count;
    new->parent = NULL; //no parent initially
    if (left != NULL) { //assign children if needed
        left->parent = new;
    }
    if (right != NULL) {
        right->parent = new;
    }
    return new;
}

//O(1)
int nodeCompare(NODE *n1, NODE *n2) {//tweak returns if needed
    if (n1->count > n2->count) {
        return 1;
    } else if (n2->count > n1->count) {
        return -1;
    }
    return 0;
}

//O(1)
int depth(NODE *n) { //use recursion to get the depth of tree
    if (n->parent == NULL) { //base case (reached the top of the tree)
        return 0;
    } else {
        return 1 + depth(n->parent);
    }
}

//O(n)
int main(int argc, char *argv[]) {
    assert(argc == 3); //make sure we have 3 arguments
    int chars[257];    //space for each char
    int i; //counter for loops
    for (i = 0; i < 257; i++) {
        chars[i] = 0;
    }

    NODE *leaves[257]; //node for each char
    for (i = 0; i < 257; i++) { //init all to null
        leaves[i] = NULL;
    }

    int currentChar; //current char as an int, used to index the array
    FILE *fp = fopen(argv[1], "rb"); //https://en.wikibooks.org/wiki/C_Programming/stdio.h/fopen
    assert(fp != NULL);

    while ((currentChar = getc(fp)) != EOF) { //read until end
        chars[currentChar]++; //increment the value of the appropriate char from the file
    }
    fclose(fp); //close the file

    PQ *pq = createQueue(*nodeCompare); //queue to hold trees

    for (i = 0; i < 257; i++) {
        if (chars[i] > 0) { //more than one char found in the file
            leaves[i] = makeNode(chars[i], NULL, NULL);
            addEntry(pq, leaves[i]); //add the entry to the PQ
            //this creates a tree, which will later be collapsed into the huffman tree
        }
    }

    //put EOF char at the end
    NODE *endOfFile = makeNode(0, NULL, NULL);
    leaves[256] = endOfFile; //last index
    addEntry(pq, endOfFile); //add the entry

    while (numEntries(pq) > 1) { //shrink the PQ until you reach only 1 tree (the huffman tree)
        NODE *right = removeEntry(pq); //remove the two lowest priority trees
        NODE *left = removeEntry(pq);
        NODE *huffman = makeNode(left->count + right->count, left, right); //make new tree with left and right children
       addEntry(pq, huffman);
    }

    int ndDepth, bits; //depth and bit for each char
    int bitSum = 0;
    for (i = 0; i < 257; i++) { //go through each index
        if (leaves[i] != NULL) { //only operate on nodes that have count
            ndDepth = depth(leaves[i]);        //find the depth to the node
            bits = ndDepth * leaves[i]->count; //we encode as the value of the char * the depth
            if (isprint(i)) { //print, based on format [see directions]
                printf("'%c': ", i);
            } else {
                printf("'%03o': ", i);
            }
            printf("%d x %d bits = %d bits \n", chars[i], ndDepth, bits); //match format on slides!
            bitSum += bits;
        }
    }
    printf("Total Bits: %d \n", bitSum);
    pack(argv[1], argv[2], leaves);
    return EXIT_SUCCESS;
}
