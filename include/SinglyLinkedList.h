#ifndef SINGLY_LINKED_LIST_H
#define  SINGLY_LINKED_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WORD_SIZE 100
#define BUFFER_SIZE 250

#define NOT_FOUND 1
#define FOUND 0

typedef struct SinglyinkedList {
    char *str;
    char found_flag;
    struct SinglyinkedList *next;
} sllist_t;

typedef struct SLLDescriptor {
    sllist_t *head;
    size_t size; 
} sll_descriptor_t;

void SLLInit (sll_descriptor_t **list_p);
void SLLPrepend (sll_descriptor_t **list_p, const char *str);
int SLLPrint (const sllist_t *node_p);
int SLLDeallocate (sllist_t *node_p);
sll_descriptor_t *LoadFileFromDiskIntoList (const char *filename_p);
int ProcessList (sll_descriptor_t *list_p);
int SaveResultsToFile (char *filename, int total_nwords, int failed_nwords, double verif_time, sllist_t *list_p);

#endif