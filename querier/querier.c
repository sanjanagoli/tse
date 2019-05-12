/*
* Sanjana Goli, April 2019
* Lab 6 (Tiny Search Engine) - querier.c
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "webpage.h"
#include "counters.h"
#include "word.h"

typedef struct docscore docscore_t;
void query(index_t *index);
counters_t* calculate_score(char **words, index_t *index, int numWords);
void counters_compare_helper(void *arg, const int key, const int count);
counters_t * evaluate_and_sequence(char **andwords, index_t *index, int numAndWords);
void update_document_scores(counters_t *documentCounter, counters_t *andsequencectr);
void update_helper(void *arg, const int key, const int count);
//void counters_intersection(counters_t *intersection, counters_t *ctr, index_t *index, char **words);
void counters_intersection(counters_t *intersection, counters_t *ctr);
void counters_intsersection_helper(void *arg, const int key, const int count);
void add_firstword_counter(void *arg, const int key, const int count);
docscore_t ** sort_matches(counters_t *ctr);
void itemcount(void *arg, const int key, const int count);
void min_helper(void *arg, const int key, const int count);
void first_min_helper(void *arg, const int key, const int count);
void insert_array_helper(void *arg, const int key, const int count);
void delete_array(docscore_t **sortedArray, int numItems);
char ** read_input();


typedef struct counters_group {
	counters_t *firstCounter;
	counters_t *secondCounter;
} counters_group_t;

typedef struct docscore {
	int docId;
	int score;
	
} docscore_t;

int
main(int argc, char *argv[])
{
	if (argc == 3) {
		//directory produced by crawler
		char *directory = (char *)malloc((strlen(argv[1])+1)*sizeof(char));
		strcpy(directory, argv[1]);

		//file produced by indexer
		char *indexFilename = (char *)malloc((strlen(argv[2])+1)*sizeof(char));
		strcpy(indexFilename, argv[2]);

		// char *directory = (char *)malloc((strlen(argv[2])+1)*sizeof(char));
		// strcpy(directory, argv[2]);

		// //file produced by indexer
		// char *indexFilename = (char *)malloc((strlen(argv[3])+1)*sizeof(char));
		// strcpy(indexFilename, argv[3]);

		int lines;
		int sizeOfTable;



		//also checks if indexFilename exists
		if ((lines = get_num_lines(indexFilename)) != -1) {
			//determines the size of the hashtable based on words in file
			if (lines >= 1000) {
				sizeOfTable = (int) (lines/10);
			} else {
				sizeOfTable = lines;
			}

		} else {
			fprintf(stderr, "The file %s did not exist\n", indexFilename);

			free(directory);
			free(indexFilename);
			return 1;
		}

		//checks if the directory was produced by crawler
		if (!isCrawlerDirectory(directory)) {
			free(directory);
			free(indexFilename);
			return 1;
		}


		//load the index from indexFilename into an internal data structure, with number of slots in hashtable
		//dependent on number of words in file
		index_t *index = index_new(sizeOfTable);
		index_load(indexFilename, index);

		// char *words[] = {"mobisys", "ali", "eric"};
		// counters_t *ctr = evaluate_and_sequence(words, index);
		// int check = counters_get(ctr, 3);
		// printf("%d", check);
		
		// counters_t *counter = index_find(index, "afsaneh");
		// printf("afsenah: %d", counters_get(counter, 3));

		query(index);

		index_delete(index);
		free(directory);
		free(indexFilename);

		// counters_t *ctr;
		// if ((ctr = index_find(index, test)) != NULL) {
		// 	counters_iterate(ctr, stdout, print_count_helper);
		// }

		

	} else {
		//too few or too many arguments
		fprintf(stderr, "usage: ./querier pageDirectory indexFilename");
		return 1;
	}

	// counters_t *intersection = counters_new();
	// counters_set(intersection, 4, 3);
	// counters_set(intersection, 5, 1);
	// counters_set(intersection, 7, 6);

	// counters_t *counter2 = counters_new();
	// counters_set(counter2, 5, 2);
	// counters_set(counter2, 6, 1);
	// counters_set(counter2, 4, 2);

	// counters_intersection(intersection, counter2);
	// int check = counters_get(intersection, 5);
	// printf("%d", check);
	

	//query();
}

void
query(index_t *index)
{
	//char *line = freadlinep(stdin);
	char *line;
	counters_t *documentScores;
	docscore_t **sortedArray;
	while( (line = freadlinep(stdin)) != NULL) {

	
		line = NormalizeWord(line);
		printf("Query: %s\n", line);

		char delim[] = " ";
		char *words[60]; // max number of commands
		char *ptr = strtok(line, delim);
		int j = 0;

		while(ptr != NULL)
		{	
			words[j] = ptr;
			//printf("%s\n", words[j]);
			ptr = strtok(NULL, delim);
			j++;
		}

		int numWords = j;
		printf("%d", numWords);
		int i = 0;
		while (i < j)
		{
			for (int k = 0; k < strlen(words[i]); k++) {
				if( !isalpha(words[i][k]) && !isspace(words[i][k]) ) {
					fprintf(stderr, "Error: bad character '%c' in query\n", words[i][k]);
					//do not proceed after this
				}

			}
			i++;
		}

		//i = 0;

		

		//return words;
		documentScores = calculate_score(words, index, numWords);
		sortedArray = sort_matches(documentScores);
		int numItems = 0;
		counters_iterate(documentScores, &numItems, itemcount);
		free(line);
		delete_array(sortedArray, numItems);
		

	}
	counters_delete(documentScores);
	free(line);
	
	
}


counters_t * 
calculate_score(char **words, index_t *index, int numWords)
{
	bool success = true;
	//int score = 0;
	if ((strcmp(words[0], "and") == 0) || (strcmp(words[0], "or") == 0)) {
		fprintf(stderr, "'%s' cannot be the first word\n", words[0]);
		success = false;
	} 

	int i = 0;

	//keeps track of the scores of all the documents overall
	counters_t *documentScores = counters_new();

	char *andsequence[30];
	int j = 0;
	//only continue as long as there are more words and no errors
	while ((i < numWords) && (success))
	{
		//confirms that "and" & "or" are not adjacent
		if (i < numWords-1) {
			if ( (strcmp(words[i], "and") == 0) && ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) ) {
				fprintf(stderr, "'%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
				break;
				success = false;
			}
			if ( (strcmp(words[i], "or") == 0) && ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) ) {
				fprintf(stderr, "'%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
				break;
				success = false;
			}
		}
		
		if (i == numWords-1) {
			if ( (strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0) ) {
				fprintf(stderr, "Error: %s cannot be at the end of the query !", words[i]);
				break;
				success = false;
			}
		}

		//if the next word is or
		if ((i < numWords-1) && (strcmp(words[i+1], "or") == 0)) {
			//evaluate and-sequence
			andsequence[j] = words[i];
			counters_t *tempCtr = evaluate_and_sequence(andsequence, index, j+1);
			
			update_document_scores(documentScores, tempCtr);

			j = 0;
			if (i == numWords-2) {
				if ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) {
					fprintf(stderr, "Error: %s cannot be at the end of the query!", words[i]);
					break;
					success = false;
				}
			} else {
				i++;
			}
			
			
		} else {
			//add word to and sequence (concatentenate word + space)
			andsequence[j] = words[i];
			if (i+1 == numWords) {
				counters_t *tempCtr = evaluate_and_sequence(andsequence, index, j+1);
				update_document_scores(documentScores, tempCtr);
				printf("other: \n");
				counters_print(documentScores, stdout);
			}
			j++;
		}
		i++;
		
	}
	return documentScores;
}

counters_t *
evaluate_and_sequence(char **andwords, index_t *index, int numAndWords)
{
	// char delim[] = " ";
	// char *andwords[60]; // max number of commands
	// char *ptr = strtok(andsequence, delim);
	// int j = 0;

	// while(ptr != NULL)
	// {	
	// 	andwords[j] = ptr;
	// 	ptr = strtok(NULL, delim);
	// 	j++;
	// }

	//counter to handle the values in the andSequence
	counters_t *andsequenceCtr = counters_new();

	
	if (andwords[0] != NULL) {
		//copies first word's counter to overall andsequencectr (intersection holder)
		
		counters_t *wordCounter = index_find(index, andwords[0]);

		if (wordCounter != NULL) {
			counters_iterate(wordCounter, andsequenceCtr, add_firstword_counter);
		}
		

		//start after the first word
		int i = 1;
		while (i < numAndWords)
		{
			counters_t *tempCtr = index_find(index, andwords[i]);
			if (tempCtr != NULL) {
				counters_intersection(andsequenceCtr, tempCtr);
				
			}
			i++;
		}
	}

	return andsequenceCtr;
}

void
add_firstword_counter(void *arg, const int key, const int count)
{
	counters_t *ctr = arg;
	counters_set(ctr, key, count);
}

void
update_document_scores(counters_t *documentCounter, counters_t *andsequencectr)
{
	counters_group_t *group = malloc(sizeof(counters_group_t));

	group->firstCounter = andsequencectr;
	group->secondCounter = documentCounter;
	counters_iterate(andsequencectr, group, update_helper);
}

void
update_helper(void *arg, const int key, const int count)
{
	counters_group_t *group = arg;
	counters_set(group->secondCounter, key, counters_get(group->secondCounter, key) + counters_get(group->firstCounter, key));
}


void
counters_intersection(counters_t *intersection, counters_t *ctr)
{
	counters_group_t *group = malloc(sizeof(counters_group_t));

	group->firstCounter = ctr;
	group->secondCounter = intersection;

	//updates intersection counter based on specifications
	counters_iterate(intersection, group, counters_intsersection_helper);

}


void
counters_intsersection_helper(void *arg, const int key, const int count)
{
	//need to pass in group of counters for comparison
	counters_group_t *group = arg;
	int newCount;
	//if the value of the compared counter's key is less than replace the value in intersection
	if ((newCount = counters_get(group->firstCounter, key)) < count) {
		counters_set(group->secondCounter, key, newCount);
	}
}

docscore_t **
sort_matches(counters_t *ctr)
{
	//determine the number of elements in the counter
	int numItems = 0;
	counters_iterate(ctr, &numItems, itemcount);
	docscore_t **sortedArray = calloc(numItems, sizeof(docscore_t*));
	// sorter_t *sorter = malloc(sizeof(sorter_t));
	// sorter->index = 0;
	// sorter->numInts = numItems;
	// sorter->array = sortedArray;

	counters_iterate(ctr, sortedArray, insert_array_helper);
	//sortedArray = sortedArray-numItems+1;
	// for (int k = 0; k < numItems; k++) {
	// 	printf("sortedarray[%d] key: %d", k, sortedArray[k]->docId);
	// 	printf("sortedarray[%d] val: %d \n", k, sortedArray[k]->score);
	// }

	//sorts the array of docscores
	for (int i = 0; i < numItems; ++i) {
        for (int j = i + 1; j < numItems; ++j) {
            if (sortedArray[i]->score < sortedArray[j]->score) {

                int score = sortedArray[i]->score;
                int doc = sortedArray[i]->docId;
                sortedArray[i]->score = sortedArray[j]->score;
                sortedArray[i]->docId = sortedArray[j]->docId;
                sortedArray[j]->score = score;
				sortedArray[j]->docId = doc;
            }

        }
    }

    for (int k = 0; k < numItems; k++) {
		printf("sortedarray[%d] key: %d", k, sortedArray[k]->docId);
		printf("sortedarray[%d] val: %d \n", k, sortedArray[k]->score);
	}

	// int minKey = 10000, i = 0;



	// while (i < numItems) {
	// 	//pass in value --> get key in return
	// 	if (i == 0) {
	// 		counters_iterate(ctr, &minKey, first_min_helper);
	// 	} else {
	// 		counters_iterate(ctr, &minKey, min_helper);
	// 	}
		
	// 	//array of keys
		
	// 	sortedArray[i] = minKey;
	// 	minKey = counters_get(ctr, minKey);
	// 	printf("after iterate minval %d\n", minKey);
	// 	i++;
	// }
	// for (int j = 0; j < numItems; j++) {
	// 	printf("%d ", sortedArray[j]);
	// }
	return sortedArray;
}

void 
insert_array_helper(void *arg, const int key, const int count)
{
	docscore_t **sortedArray = arg;
	docscore_t *docscore = malloc(sizeof(docscore_t));
	docscore->docId = key;
	docscore->score = count;

	int i = 0;
	while (sortedArray[i] != NULL) {
		i++;
	}
	sortedArray[i] = docscore;
	printf("sortedArray[%d] key: %d, count: %d\n", i, sortedArray[i]->docId, sortedArray[i]->score);

	// docscore_t *docscore = malloc(sizeof(docscore_t));
	// printf(" key: %d", sortedArray->docId);
	// printf(" val: %d \n", sortedArray->score);
	// docscore->docId = key;
	// docscore->score = count;
	// printf("docscore docid: %d \n", key);
	
	// *sortedArray = *docscore;
	// if(key == 4) {
	// 	printf("here: %d", sortedArray->docId);
	// 	printf("also: %d \n", sortedArray->score);
	// 	//printf("another: %d \n", (sortedArray-1)->docId);
	// }

	//printf("address %p\n", sortedArray);
	//sortedArray = sortedArray + sizeof(docscore);
	
}

void
delete_array(docscore_t **sortedArray, int numItems)
{
	
	for(int i = 0; i < numItems; i++) {
		free(sortedArray[i]);
	}
	free(sortedArray);
}


void 
itemcount(void *arg, const int key, const int count)
{
  int *nitems = arg;

  if (nitems != NULL && (key >= 0) && (count >= 0))
    (*nitems)++;
}

void
first_min_helper(void *arg, const int key, const int count)
{
	int *minKey = arg;
	
	if (*minKey > count) {
		
		*minKey = key;
	}
}

void
min_helper(void *arg, const int key, const int count)
{
	int *minKey = arg;
	printf("reached here: %d %d minVal %d \n", key, count, *minKey);
	if (*minKey >= count && *minKey != key) {
		
		*minKey = key;
	}
}





/************** GRAVEYARD *********************/

void
counters_compare_helper(void *arg, const int key, const int count)
{	
	//iterating through the next word's counter
	counters_t *tempCtr = arg;
	//if the word has a lower document count than the one in the total counter, replace
	//if the word 
	int newCount;
	if ((newCount = counters_get(tempCtr, key)) < count) {
		//counters_set(andsequenceCtr, key, newCount);
	}

}

// char ** 
// read_input()
// {
// 	char *line = freadlinep(stdin);
	
// 	int init_size = strlen(line);
// 	char delim[] = " ";
// 	char *words[20]; // max number of commands
// 	char *ptr = strtok(line, delim);
// 	int j = 0;
// 	while(ptr != NULL)
// 	{
		
// 		words[j] = ptr;
// 		ptr = strtok(NULL, delim);
// 		j++;
// 	}

// 	int i = 0;
// 	while (words[i] != NULL)
// 	{
// 		printf("%s\n", words[i]);
// 		i++;
// 	}
// 	return words;
// }

