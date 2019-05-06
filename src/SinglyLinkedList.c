#include "../include/SinglyLinkedList.h"

void SLLInit (sll_descriptor_t **list_p)
{
    *list_p = malloc (sizeof (sll_descriptor_t));
    (*list_p)->size = 0;
    (*list_p)->head = NULL;
}

void SLLPrepend (sll_descriptor_t **list_p, const char *str)
{
    sllist_t *new_node_p;
    int str_len;

    str_len = strlen (str);
    // There is also the null character
    str_len++;

    new_node_p = malloc (sizeof (sllist_t));

    // Makes room for the string according to its size
    new_node_p->str = malloc (sizeof (char) * str_len);

    // Initializing the quant data. Here, it serves as a flag and the program 
    // starts off by assuming that all words from the file were FOUND in the dict.
    new_node_p->quant = FOUND;

    // Copys the string into the node data
    strcpy (new_node_p->str, str);
    
    if ((*list_p)->head == NULL) {
        new_node_p->next = NULL;
        (*list_p)->head = new_node_p;
        (*list_p)->size = 1;
    } else {
        new_node_p->next = (*list_p)->head;
        (*list_p)->head = new_node_p;
        (*list_p)->size++;
    }
}

int SLLPrint (const sllist_t *node_p)
{
    const sllist_t *temp_p = node_p;

    while (temp_p != NULL) {
        // if (temp_p->quant == FOUND)
         printf ("%s", temp_p->str);
         printf ("%d\n", temp_p->quant);
        temp_p = temp_p->next;
    }
}

int SLLDeallocate (sllist_t *node_p)
{
    if (node_p == NULL)
        return -1;
    else {
        SLLDeallocate (node_p->next);
        free (node_p->str);
        free (node_p);
    }

    return 0;
}

sll_descriptor_t *LoadFileFromDiskIntoList (const char *filename_p)
{
    FILE *file_p;
    char buffer[BUFFER_SIZE];
    char ch, ch_prev;
    size_t buffer_counter; // Counts the characters in the buff
    sll_descriptor_t *list_p;
    
    SLLInit (&list_p);

    buffer_counter = 0;

    file_p = fopen (filename_p, "r");

    if (file_p == NULL)
        return NULL;

    ch_prev = '\0';
    while ( (ch = fgetc (file_p)) != EOF) {

        // When it happens, buff has a word formed
        if (isalpha(ch_prev) && (ch == ' ' || ch == '\n')) {
            buffer[buffer_counter] = '\0';
            buffer_counter = 0;
            SLLPrepend (&list_p, buffer);
            //printf ("%d\n", i++);
        } else if (isalpha(ch) || ( isalpha(ch_prev) && (ch == '-' || ch == '\''))){
            buffer[buffer_counter] = ch;
            buffer_counter++;
        }
        ch_prev = ch;
    }
    
    fclose (file_p);

    return list_p;
}

int ProcessList (sll_descriptor_t *list_p)
{
    sllist_t *node_p, *temp_p, *aux_p;

    // If list is empty, there's nothing to be done here
    if (list_p == NULL)
        return -1;

    node_p = list_p->head;

    while (node_p != NULL && node_p->next != NULL) {

        // All nodes with quant equals to FOUND are removed. All but the fisrt, actually
        if (node_p->next->quant == FOUND) {
            temp_p = node_p->next;
            node_p->next = node_p->next->next;

            // Firstly, it deallocates the string space
            free (temp_p->str);

            // Then, it deallocates the node space
            free (temp_p);
            list_p->size--;
        } else 
            node_p = node_p->next;
    }

    // If the quant data of the first node is FOUND, remove it from the list
    if (list_p->head != NULL && list_p->head->quant == FOUND) {
        temp_p = list_p->head;
        list_p->head = list_p->head->next;

        // Firstly, it deallocates the string space
        free (temp_p->str);

        // Then, it deallocates the node space
        free (temp_p);
        list_p->size--;
    }

    // Counts the words and removes the duplicated ones
    node_p = list_p->head;
    while (node_p != NULL) {
        temp_p = node_p;
        while (temp_p->next != NULL){
            // While the strings from node_p and temp_p->next are equal, keep removing temp_p->next
            if (!strcmp(node_p->str, temp_p->next->str)) {
                // Now quant serves as a counter
                node_p->quant++;
                aux_p = temp_p->next;
                temp_p->next = temp_p->next->next;
                free (aux_p->str);
                free (aux_p);
                list_p->size--;
            }
            // When they are different, go to the next one
            else 
                temp_p = temp_p->next;
        }
        node_p = node_p->next;
    }

    return 0;
}

int SaveResultsToFile (char *filename_p, int total_nwords, int failed_nwords, double verif_time, sllist_t *list_p)
{
    FILE *result_file_p;
    sllist_t *temp_p;

    result_file_p = fopen (filename_p, "w");

    if (result_file_p == NULL)
        return -1;

    fprintf (result_file_p, "Total number of words in the text: %d\n", total_nwords);
    fprintf (result_file_p, "Total time of verification: %.2fms\n", verif_time*1000);
    fprintf (result_file_p, "Number of words that failed on spell checking: %d\n", failed_nwords);
    fprintf (result_file_p, "List of of words that failed on spell checking: \n");
    fprintf (result_file_p, "[number of ocurrences - word]\n");
    fprintf (result_file_p, "------------------------------------------------------------\n");

    temp_p = list_p;
    while (temp_p != NULL) {
        fprintf (result_file_p, "%5d - %s\n", temp_p->quant, temp_p->str);
        temp_p = temp_p->next;
    }

    fclose (result_file_p);
    return 0;
} 