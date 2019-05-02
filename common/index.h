/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - index.h
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include <string.h>
#include "hashtable.h"
#include "pagedir.h"
#include "counters.h"
#include "file.h"

typedef struct index index_t;  // opaque to users of the module

index_t * index_new();

void index_insert_word(index_t *index, const char *word, int pageId);

void index_save(char *filename, index_t index);

void index_delete(index_t index);

void index_build(char *directory, index_t *index);

char * createFilename(int id, char *dirname);

