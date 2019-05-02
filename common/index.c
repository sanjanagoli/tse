/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - index.c
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
#include "file.h"
#include "counters.h"
#include "index.h"

typedef struct index {
	hashtable_t *hashtable;
} index_t;

index_t *
index_new()
{
  	index_t *index = malloc(sizeof(index_t));
  	if (index != NULL) {
  		index->hashtable = hashtable_new(250);
  		return index;
  	} else {
  		return NULL;
  	}
}

void
index_insert_word(index_t *index, const char *word, int pageId)
{	
	//each word needs a counterset --> key: pageID, val: count of the word in the page
	
	if (!hashtable_find(index->hashtable, word)) {
		counters_t *ctr = counters_new();
		if (!hashtable_insert(index->hashtable, word, ctr) ) {
			free(ctr);
		}
	} else {
		counters_t *counter = hashtable_find(index->hashtable, word);
		counters_add(counter, pageId);
	}

}

void
index_delete(index_t index)
{

}

void
index_build(char *directory, index_t *index)
{
    //creates first file to check
    int id = 1;
	char *filename = createFilename(id, directory);

    FILE *fp;
    
    while ((fp = fopen(filename, "r")) != NULL) {
    	

    	//create webpage from url at the top --> need to use webpage_getNextWord
    	char *url = malloc(250*sizeof(char));
    	fgets(url, 150, fp);
    	webpage_t *page = webpage_new(url, 0, NULL);
    	printf("%s\n", url);

    	if (webpage_fetch(page)) {
    		printf("HTML was fetched");
    	} else {
    		fprintf(stderr, "ERROR: HTML COULD NOT BE FETCHED");
    		exit(1);
    	}

    	int pos = 0;
  		char *result;
 
		while ((result = webpage_getNextWord(page, &pos)) != NULL) {
		   printf("Found word: %s\n", result);

		   //calls method above which increments counter of word based on corresponding id
		   index_insert_word(index, result, id);
		   
		}	


    	//need to free filename that was previously malloc and create new path
    	id++;
    	free(filename);
    	fclose(fp);


    	filename = createFilename(id, directory);
 
    }
    


}

void
index_save(char *filename, index_t index)
{

}

/* createFilename creates filename of format dirname/id 
 * free(filename) needs to be called after method is invoked */
char *
createFilename(int id, char *dirname)
{
	
	char strId[4];
	char *filename = (char *)malloc((strlen(dirname)+4)*sizeof(char));	

	sprintf(strId, "%d", id);
	strcpy(filename, dirname);
	strcat(filename, "/");
	strcat(filename, strId);

	return filename;

}