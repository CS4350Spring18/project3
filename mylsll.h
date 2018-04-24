// C program for generic linked list

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A linked list node */
typedef struct Node {
    char *data;
    struct Node *next;
} Node;

/* Insert the nodes in alphabetical order. */
void insert(Node **headPtr, char *new_data, size_t data_size);

/* Print the list in-order.*/
void printList(Node *node, void (*fptr)(void *));

/* Traverse all the nodes. Frees both data and
 * the node. */
void freeNodes(Node *node);

/* Traverse all the nodes. Frees both data and
 * the node. */
void printLine(void *s);

