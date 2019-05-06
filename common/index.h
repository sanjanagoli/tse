/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - index.h
*
* index.h contains a library of function that are called in indexer.c and indextest.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "webpage.h"
#include <string.h>
#include "hashtable.h"
#include "pagedir.h"
#include "counters.h"
#include "file.h"

typedef struct index index_t;  // opaque to users of the module

/* index_new: takes in number of slots to initailize the hashtable
   num_slots depends on whether the caller knows how many words/keys are going to be inputted
   hashtable contains several sets with keys that are words and values that are counters
   counters correspond to pageId and wordcount values
*/
index_t * index_new(int num_slots);

/* index_insert_word: takes in word and pageId to confirm whether they are valid inputs (pageId > 0)
   if the word does not exist, create a counter for the word and insert into hashtable
   if the word already exists, updated counter
   to be called in index_build
*/
void index_insert_word(index_t *index, const char *word, int pageId);

/* index_set_wordcount: takes in word and pageId to confirm whether they are valid inputs (pageId > 0)
   if the word does not exist, create a counter for the word and insert into hashtable with key and value combination
   to be called in index_load
*/
void index_set_wordcount(index_t *index, const char *word, int pageId, int countVal);

/* index_save: create new writable file and print word, pageId, and count in format specfied below */
void index_save(char *filename, index_t *index);

/* get_num_lines: call lines_in_file from file.c in order to determine the size of the hashtable in index data structure */
int get_num_lines(char *filename);

/* print_word_counter iterates through the counter to print id and count in the format specified below */
void print_word_counter(void *arg, const char *key, void *item);

/* print_count_helper is a method passed into counter_iterate to print id and count in this format: word id count id count */
void print_count_helper(void *arg, const int key, const int count);

/* index_load: loads in words from input file to index data structure, calls index_set_wordcount */
void index_load(char *filename, index_t *index);

/* index_delete: frees memory allocated to the data structure --> frees index itself and hashtable of data structure */
void index_delete(index_t *index);

/* delete_helper: helper method is necessary for hashtable_delete --> frees each of the counters in hashtable with counters_delete */
void delete_helper(void *item);

/* index_build: scans file that is inputted, creates a webpage from url and html given, parses html to find words,
   inserts words into index using index_insert_word */
void index_build(char *directory, index_t *index);

/* createFilename: creates filename of format dirname/id free(filename) needs to be called after method is invoked */
char * createFilename(int id, char *dirname);

