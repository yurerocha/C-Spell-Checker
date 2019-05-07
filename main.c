#include "include/HashTableFuncs.h"
#include "include/SinglyLinkedList.h" 

int main (int argc, char *argv[])
{   
    sll_descriptor_t *buffer_p;
    hash_table_t *hashtable_p;
    clock_t start, end;
    double cpu_time;
    char dict_filename_p[MAX_WORD_SIZE], check_filename_p[MAX_WORD_SIZE];
    int check;
    int total_nwords;

    strcpy (dict_filename_p, "dictionary/dict.txt");

    hashtable_p = malloc (sizeof (hash_table_t));
    InitHashTable (hashtable_p);

    // Creates the hash table
    puts ("(!)Creating Hash Table(!)");
    check = CreateHashTable (dict_filename_p, hashtable_p);

    if (!check)
        puts ("(!)Done creating Hash Table(!)");
    else {
        puts ("(!)Failed creating Hash Table(!)");
        perror (dict_filename_p);
        return -1;
    }

    // File name
    if (argc <= 1) {
        printf ("File to check: ");
        fgets (check_filename_p, MAX_WORD_SIZE, stdin);

        // Removes the \n at the end of the string
        check_filename_p[strlen (check_filename_p) - 1] = '\0';
    } else 
        strcpy (check_filename_p, argv[1]);

    // Loads words into memory
    puts ("(!)Loading file(!)");
    buffer_p = LoadFileFromDiskIntoList (check_filename_p);

    // Saves the total number of words to a variable
    total_nwords = buffer_p->size;

    if (buffer_p != NULL)
        puts ("(!)Done loading file(!)");
    else {
        puts ("(!)Failed loading file(!)");
        perror (check_filename_p);
        return -1;
    }

    // Checking check filename_p against the dict file and measures the time 
    start = clock();
    CheckHashTable (hashtable_p, buffer_p->head);
    end = clock();

    // Calculating the cpu_time to check 
    cpu_time = (end-start) / (double) CLOCKS_PER_SEC;

    puts ("(!)Processing results(!)");
    // Elimitating words that were found in the dict file
    ProcessList (buffer_p);
    puts ("(!)Done processing results(!)");

    puts ("(!)Saving results to file(!)");
    SaveResultsToFile ("result.txt", total_nwords, buffer_p->size, cpu_time, buffer_p->head);
    puts ("(!)Done saving results to file(!)");

    // Deallocating memory
    SLLDeallocate (buffer_p->head);
    free (buffer_p);
    free (hashtable_p);

    return 0;
}
