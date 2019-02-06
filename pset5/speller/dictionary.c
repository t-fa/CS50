// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor = root;
    int alph_index;

    int len = strlen(word);
    for(int j = 0; j < (len + 1); j++)
    {
        if(word[j] != '\0')
        {
            // apostrophe check
            if(word[j] == '\'')
            {
                alph_index = 26;
            }
            else
            {
                alph_index = tolower(word[j]) - 'a';
            }

            if(cursor->children[alph_index] != NULL)
            {
                // letter is in trie
                cursor = cursor->children[alph_index];
            }
            else if(cursor->children[alph_index] == NULL)
            {
                // misspelled
                return false;
            }
        }
        else // char is \0
        {
            return cursor->is_word;
        }
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // file handling
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // memory for root
    root = calloc(sizeof(node), 1);
    if(!root)
    {
        printf("Insufficient memory for root.\n");
        fclose(file);
        return false;
    }

    // temporary inserter pointer
    node *temp = root;

    // go through each letter in dictionary, one character a time
    for(int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        // all words in dictionary are lowercase, so this gives us an "alphabetical" index
        int i = c - 97;
        // different index for apostrophe
        if(c == '\'')
        {
            i = 26;
        }

        // end of word
        if(c == '\n')
        {
            word_count++; // size function
            temp->is_word = true;
            temp = root;
        }
        // node null
        else if(temp->children[i] == NULL)
        {
            temp->children[i] = calloc(sizeof(node), 1);
            if(temp->children[i] == NULL) // malloc failed
            {
                printf("Insufficient memory for temp->children[i].\n");
                fclose(file);
                return false;
            }
            temp = temp->children[i];
        }
        // move on
        else
        {
            temp = temp->children[i];
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool recursive_free(node *ptr)
{
    node *nav = ptr;
    for(int i = 0; i < 27; i++)
    {
        if(nav->children[i] != NULL)
        {
            recursive_free(nav->children[i]);
        }
    }
    free(ptr);
    return true;
}

bool unload(void)
{
    return recursive_free(root);
}
