// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// notes about nodes
// 1. node cursor - var with 2 values (word, next)      // node *cursor - pointer to node
// 2. struct node *next, or node *next - a pointer, which contains an address, and it is "half" of a node, without word
// 3. 2 is for understanding node *cursor, it doesnt have word itself, but it points to another node which has word (cursor->word) and pointer (cursor->next)
// 4. node *cursor, or struct node *cursor is that red part of a node from the Walkthrought illustrations, which is a pointer to another node
// 5. cursor and cursor->next are the same datatype (pointer to node) with the same size

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// to count the number of words when loading
unsigned int count = 0;

// Hash table
node *table[N];


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    node *cursor = table[hash(word)];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    // you should not do free(cursor); because you didnt allocate any memory

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)     // if (!fp)
    {
        return false;
    }

    char temp_word[LENGTH + 1];
    unsigned int index;

    while (fscanf(fp, "%s", temp_word) != EOF)
    {
        node *new = malloc(sizeof(node));   // ! it allocates memory for the node (which has word and next), and returns the address (!) of that node
        strcpy(new->word, temp_word);
        // but is there a memory for new->word, new->next (yes)     // maybe there is a pointer to node, but there is no node itself (no)
        // malloc creates a node, memory for it, and returns its address

        index = hash(new->word);
        new->next = table[index];
        table[index] = new;

        count++;
        // free(new);   // this was my main mistake !   // and confusing cursor (pointer to a node) and cursor->next (pointer to a node inside 1st node)
        // does this delete inserted node? maybe this is why there is a memory leak and it doesnt work, because chain is broken (yes)
        // how free() works in arrays or string - when you free a pointer, it goes to the address of pointer and frees everything there
        // think how the value of new is changing when you free it in every cycle, and how it will change if you dont free it
        // new is a pointer, which has an address of an allocated memory for the node (which has word and next, sizeof(node))
        // if you do free(new), it will go where new is pointing to (new is a pointer which holds an address of node), and it will free/delete what is in there (the node you created)
    }

    fclose(fp);

    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO     // try with recursion ?
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;     // you should not malloc memory, because malloc will create a new node      // tmp is used to point to the already existing node
            cursor = cursor->next;  // you should do it before free(tmp), because free(tmp) deletes the address in cursor->next (and cursor->word too)
            free(tmp);              // frees the node to which tmp points to
        }
        // you should not do free(cursor); because you didnt allocate any memory
    }
    return true;
    //return false;
}
