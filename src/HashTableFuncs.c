#include "../include/HashTableFuncs.h"

// Sets all positions of the hash table to 0
void InitHashTable (hash_table_t *hash_table_p)
{
    unsigned int i;

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        hash_table_p->slot[i] = 0;
    }
}

// Kinda prehash function
unsigned int SDBMHash (const char *word)
{
	unsigned int hash = 0;

	while (*word != '\0') {
		hash = (*word) + (hash << 6) + (hash << 16) - hash;
        word++;
    }

	return hash;
}

unsigned int HashFunction (unsigned int key, unsigned int i)
{
    return ((key % HASH_TABLE_SIZE) + i*(1 + key % (HASH_TABLE_SIZE-1))) % HASH_TABLE_SIZE;
}

// Se considerássemos as palavras diretamente como keys, seria difícil saber found_flago espaço reservar para cada.
// Por isso, as keys são na verdade um prehash.
int InsertWordIntoHashTable (hash_table_t *hash_table_p, const char *word) 
{   
    unsigned int key, hash, i;

    key = SDBMHash (word);

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        hash = HashFunction (key, i);
        if (hash_table_p->slot[hash] == 0) {
            hash_table_p->slot[hash] = key;
            return 0;
        }
    }

    return -2;  // Didn't find space for insertion
}

int CreateHashTable (const char *filename_p, hash_table_t *hash_table_p)
{
    FILE *dict_file_p;
    unsigned char word[MAX_WORD_SIZE];
    unsigned short word_len;

    dict_file_p = fopen(filename_p, "r");

    if (dict_file_p == NULL)
        return -1;

    while (fgets(word, MAX_WORD_SIZE, dict_file_p) != NULL) {

        // Removes the \n at the end of the string
        word_len = strlen (word);
        if (word[word_len-1] == '\n')
            word[word_len-1] = '\0';

        InsertWordIntoHashTable (hash_table_p, word);
    }

    fclose (dict_file_p);
    return 0;
}

int SearchWordInHashTable (const hash_table_t *hash_table_p, const char *word)
{
    unsigned int key, hash, i;

    key = SDBMHash (word);

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        hash = HashFunction (key, i);
        if (hash_table_p->slot[hash] == key)
            return FOUND; // Found
        else if (hash_table_p->slot[hash] == 0)
            return NOT_FOUND; // Not found
    }

    return 0; // Not found. This one is probably not necessary
}

void CheckHashTable (const hash_table_t *hash_table_p, sllist_t *buffer_p) 
{
    sllist_t *temp_p = buffer_p;

    while (temp_p) {
        if (SearchWordInHashTable (hash_table_p, temp_p->str) == NOT_FOUND) {
            // found_flag value is FOUND, by default. It means that the word was FOUND in dict
            temp_p->found_flag = NOT_FOUND;
    }
            
        temp_p = temp_p->next;
    }
}