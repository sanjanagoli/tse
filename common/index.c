/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - index.c
*
* index.c contains a library of function that are called in indexer.c and indextest.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include <string.h>
#include <ctype.h>
#include "hashtable.h"
#include "pagedir.h"
#include "file.h"
#include "counters.h"
#include "word.h"
#include "index.h"

typedef struct index {
	hashtable_t *hashtable;
} index_t;

/* see index.h for documentation */
index_t *
index_new(int num_slots)
{
  	index_t *index = malloc(sizeof(index_t));

  	//as long as the memory was allocated correctly
  	if (index != NULL) {
  		index->hashtable = hashtable_new(num_slots);
  		return index;
  	} else {
  		return NULL;
  	}
}

/* see index.h for documentation */
void
index_insert_word(index_t *index, const char *word, int pageId)
{	
	//each word needs a counterset --> key: pageID, val: count of the word in the page
	if ((word != NULL) && (pageId > 0)) {
		//need to initialize a new counterset if the word has not been inputted yet
		if (!hashtable_find(index->hashtable, word)) {
			counters_t *ctr = counters_new();
			counters_add(ctr, pageId);

			//if the set does not insert correctly, need to free memory allocated
			if (!hashtable_insert(index->hashtable, word, ctr) ) {
				free(ctr);
			}
		} else {
			//if the word has already been added, updated the counter of the corresponding pageId
			counters_t *counter = hashtable_find(index->hashtable, word);
			counters_add(counter, pageId);
		}
	} else {
		fprintf(stderr, "Error: pageId must be greater than 0 and word must be valid!\n");
	}
}

/* see index.h for documentation */
void
index_set_wordcount(index_t *index, const char *word, int pageId, int countVal)
{	
	//each word needs a counterset --> key: pageID, val: count of the word in the page
	if ((word != NULL) && (pageId > 0)) {
		//need to initialize a new counterset if the word has not been inputted yet
		if (!hashtable_find(index->hashtable, word)) {
			counters_t *ctr = counters_new();
			//when using this method, the caller already has the key and the count, so just need to set value automatically
			counters_set(ctr, pageId, countVal);
			if (!hashtable_insert(index->hashtable, word, ctr) ) {
				free(ctr);
			}
		} else {
			//will probably never reach this condition because takes in input from formatted file (not html)
			counters_t *counter = hashtable_find(index->hashtable, word);
			counters_set(counter, pageId, countVal);
		}
	} else {
		fprintf(stderr, "Error: Error: pageId must be greater than 0 and word must be valid!\n");
	}
}

/* see index.h for documentation */
void
index_delete(index_t *index)
{
	//frees all memory
	if(index != NULL) {
		hashtable_delete(index->hashtable, delete_helper);
		free(index);
	}
}

/* see index.h for documentation */
void
delete_helper(void *item)
{
	if (item != NULL) {
		counters_t *ctr = item;
		counters_delete(ctr);
	}
}

/* see index.h for documentation */
void
index_build(char *directory, index_t *index)
{
    //creates first file to check
    int id = 1;
	char *filename = createFilename(id, directory);

    FILE *fp;
    
    while ((fp = fopen(filename, "r")) != NULL) {
    	
    	//create webpage from url at the top --> need to use webpage_getNextWord
    	char *url = freadwordp(fp);
    	char *depth = freadlinep(fp);
    	char *html = freadfilep(fp);

    	webpage_t *page = webpage_new(url, 0, html);

    	int pos = 0;
  		char *result;
 
		while ((result = webpage_getNextWord(page, &pos)) != NULL) {
			char *str = NormalizeWord(result);
		   	free(result);

		   //calls method above which increments counter of word based on corresponding id
		    if(strlen(str) > 3) {
		   		index_insert_word(index, str, id);
		    }
		    free(str);
		}	

    	//need to free filename that was previously malloc and create new path
    	id++;
    	free(filename);
    	free(depth);
    	webpage_delete(page);
    	fclose(fp);

    	//need to update filename for the next loop
    	filename = createFilename(id, directory);
    }

    free(filename);
}

/* see index.h for documentation */
void
index_save(char *filename, index_t *index)
{
	FILE *fp;
	//creates writable file if it doesnt exist
	if ((fp = fopen(filename, "w")) != NULL) {
		//prints words from index, iterates through each word in hashtable
		hashtable_iterate(index->hashtable, fp, print_word_counter);

	} else {
		fprintf(stderr, "Error: File %s could not be opened!\n", filename);
		exit(1);
	}

	fclose(fp);

}

/* see index.h for documentation */
void 
print_word_counter(void *arg, const char *key, void *item)
{
	FILE *fp = arg;
	if ((key != NULL) && (item != NULL)) {
		//prints word in following format: word id count id count 
		fprintf(fp, "%s ", key);
		counters_t *counter = item;

		//printing each counternode in format specified
		counters_iterate(counter, fp, print_count_helper);
		fprintf(fp, "\n");
	}
}

/* see index.h for documentation */
void
print_count_helper(void *arg, const int key, const int count)
{
	FILE *fp = arg;
	fprintf(fp, "%d %d ", key, count);

}

/* see index.h for documentation */
int
get_num_lines(char *filename)
{	
	FILE *fp;
	int returnVal;
	if ((fp = fopen(filename, "r")) != NULL) {
		returnVal = lines_in_file(fp);
		fclose(fp);
	} else {
		//if file doesn't exist
		returnVal = -1;
	}
	return returnVal;
}

/* see index.h for documentation */
void
index_load(char *filename, index_t *index)
{
	//checks if the file exists
	FILE *fp;
	if ((fp = fopen(filename, "r")) != NULL) {
		char buf[100]; 
		int key;
		int val;

		//do this so long as there are more lines in the file
		int numLines = lines_in_file(fp);
    	while (numLines > 0) {

    		//read the first word
    		if (fscanf(fp,"%s ",buf)==1) {
    			//iterate through the numbers on the rest of the line
    			while (fscanf(fp, "%d %d", &key, &val)==2) {
    				//set the counter of the word to the specified value
    				index_set_wordcount(index, buf, key, val);
    			}
    		}
    		numLines--;
    	}

	} else {
		fprintf(stderr, "Error: File %s could not be opened!\n", filename);
		exit(1);
	}

	fclose(fp);
}

/* see index.h for documentation */
char *
createFilename(int id, char *dirname)
{
	//creates filename based on id and dirname
	char strId[4];
	char *filename = (char *)malloc((strlen(dirname)+5)*sizeof(char));	

	sprintf(strId, "%d", id);
	strcpy(filename, dirname);
	strcat(filename, "/");
	strcat(filename, strId);

	return filename;

}