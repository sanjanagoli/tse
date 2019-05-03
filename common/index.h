/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - index.h
*
*
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

index_t * index_new(int num_slots);

void index_insert_word(index_t *index, const char *word, int pageId);

void index_set_wordcount(index_t *index, const char *word, int pageId, int countVal);

void index_save(char *filename, index_t *index);

int get_num_lines(char *filename);

void print_word_counter(void *arg, const char *key, void *item);

void print_count_helper(void *arg, const int key, const int count);

void index_load(char *filename, index_t *index);

void index_delete(index_t *index);

void delete_helper(void *item);

void index_build(char *directory, index_t *index);

char * createFilename(int id, char *dirname);

