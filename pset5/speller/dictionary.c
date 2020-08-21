// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"
unsigned int hash(const char *word);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

// Counter of the number of words in dictionary
unsigned int counter = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // make a copy of the word
    int n = strlen(word);
    char copy[LENGTH + 1];
    for (int i = 0; i < n; i++)
    {
        copy[i] = tolower(word[i]);
    }
    copy[n] = '\0';

    unsigned int index = hash(copy);
    node *cursor;
    cursor = table[index];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, copy) == 0)  // return true if the word is found
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // A fellow CS50 alumni shared this hash function that her husband made
    // https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // open dictionary file
    char word[LENGTH + 1];

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", word) != EOF)
    {
        // read strings from file one at a time

        // create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        // hash word to obtain a hash value
        unsigned int index = hash(word);

        // insert node into hash table with that hash value
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }

        counter++; // add to counter of words in dictionary
    }
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node *cursor = NULL;
    node *tmp = NULL;

    for (int i = 0; i < N; i++) // used to access the linked lists
    {
        if (table[i] != NULL)
        {
            cursor = table[i];
            tmp = table[i];
            while (cursor != NULL) // this will break when it reaches a NULL
            {
                // the cursor and tmp, when cursor is null break
                cursor = cursor->next;
                free(tmp);
                tmp = cursor;
            }
        }
    }
    return true;
}
