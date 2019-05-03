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

index_t *
index_new(int num_slots)
{
  	index_t *index = malloc(sizeof(index_t));
  	if (index != NULL) {
  		index->hashtable = hashtable_new(num_slots);
  		return index;
  	} else {
  		return NULL;
  	}
}

void
index_insert_word(index_t *index, const char *word, int pageId)
{	
	//each word needs a counterset --> key: pageID, val: count of the word in the page
	if ((word != NULL) && (pageId > 0)) {
		if (!hashtable_find(index->hashtable, word)) {
			counters_t *ctr = counters_new();
			counters_add(ctr, pageId);
			if (!hashtable_insert(index->hashtable, word, ctr) ) {
				free(ctr);
			}
		} else {
			counters_t *counter = hashtable_find(index->hashtable, word);
			counters_add(counter, pageId);
		}
	} else {
		fprintf(stderr, "Error: ");
	}
}

void
index_set_wordcount(index_t *index, const char *word, int pageId, int countVal)
{	
	if ((word != NULL) && (pageId > 0)) {
		if (!hashtable_find(index->hashtable, word)) {
			counters_t *ctr = counters_new();
			counters_add(ctr, pageId);
			if (!hashtable_insert(index->hashtable, word, ctr) ) {
				free(ctr);
			}
		} else {
			counters_t *counter = hashtable_find(index->hashtable, word);
			counters_set(counter, pageId, countVal);
		}
	} else {
		fprintf(stderr, "Error: ");
	}
}

void
index_delete(index_t *index)
{
	hashtable_delete(index->hashtable, delete_helper);
}

void
delete_helper(void *item)
{
	if (item != NULL) {
		counters_t *ctr = item;
		counters_delete(ctr);
	}
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
    	fgets(url, 250, fp);
    	webpage_t *page = webpage_new(url, 0, NULL);

    	if (webpage_fetch(page)) {
    		printf("HTML was fetched");
    	} else {
    		fprintf(stderr, "ERROR: HTML COULD NOT BE FETCHED");
    		exit(1);
    	}

    	int pos = 0;
  		char *result;
 
		while ((result = webpage_getNextWord(page, &pos)) != NULL) {
			char *str = (char *)malloc(sizeof(char)*(strlen(result)+1)); 
		   	str = NormalizeWord(result);
		   	free(result);
		   	//printf("result %s\n", str);

		   //calls method above which increments counter of word based on corresponding id
		    if(strlen(str) > 3) {
		   		index_insert_word(index, str, id);
		    }
		}	

    	//need to free filename that was previously malloc and create new path

    	id++;
    	free(filename);
    	free(page);
    	fclose(fp);


    	filename = createFilename(id, directory);
    }
    free(filename);
}

void
index_save(char *filename, index_t *index)
{
	FILE *fp;
	if ((fp = fopen(filename, "w")) != NULL) {
		hashtable_iterate(index->hashtable, fp, print_word_counter);

	} else {
		fprintf(stderr, "Error: File %s could not be opened!", filename);
		exit(1);
	}

	fclose(fp);

}

void 
print_word_counter(void *arg, const char *key, void *item)
{
	FILE *fp = arg;
	if ((key != NULL) && (item != NULL)) {
		fprintf(fp, "%s ", key);
		counters_t *counter = item;

		//printing each counternode in format specified
		counters_iterate(counter, fp, print_count_helper);
		fprintf(fp, "\n");
	}
}

void
print_count_helper(void *arg, const int key, const int count)
{
	FILE *fp = arg;
	fprintf(fp, "%d %d ", key, count);

}

int
get_num_lines(char *filename)
{	
	FILE *fp;
	int returnVal;
	if ((fp = fopen(filename, "w")) != NULL) {
		returnVal = lines_in_file(fp);
		fclose(fp);
	} else {
		//if file doesn't exist
		returnVal = -1;
	}
	return returnVal;
}

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
    			printf("%s", buf);
    			//iterate through the numbers on the rest of the line
    			while (fscanf(fp, "%d %d", &key, &val)==2) {
    				// if (fscanf(fp, "%d ", &val)==1) {
    				// 	//only insert into index if there is a valid key, value pair
    				// 	index_insert_word(index, buf, key);
    				// }
    				printf("%d %d ", key, val);
    				//set the counter of the word to the specified value
    				index_set_wordcount(index, buf, key, val);
    			}
    			printf("\n");
    		}
    		numLines--;
    	}

	} else {
		fprintf(stderr, "Error: File %s could not be opened!", filename);
		exit(1);
	}

	fclose(fp);
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