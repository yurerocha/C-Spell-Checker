#ifndef HASH_TABLE_FUNCS_H
#define HASH_TABLE_FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "SinglyLinkedList.h"

// 1536011 or 1075201 
#define HASH_TABLE_SIZE 1536011
#define MAX_WORD_SIZE 100

#define NOT_FOUND 1
#define FOUND 0

typedef struct HashTable {
    unsigned int slot[HASH_TABLE_SIZE];
} hash_table_t;

void InitHashTable (hash_table_t *hash_table_p);
unsigned int SDBMHash (const char* str);
unsigned int HashFunction (unsigned int key, unsigned int i);
int InsertWordIntoHashTable (hash_table_t *hash_table_p, const char *word);
int SearchWordInHashTable (const hash_table_t *hash_table_p, const char *word);
int CreateHashTable (const char *filename_p, hash_table_t *hash_table_p);
void CheckHashTable (const hash_table_t *hash_table_p, sllist_t *buffer_p) ; 

#endif 
