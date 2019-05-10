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

void query();
void calculate_score(char **words);
void counters_compare_helper(void *arg, const int key, const int count);
void evaluate_and_sequence(char *andsequence, index_t *index, counters_t *ctr);
//void counters_intersection(counters_t *intersection, counters_t *ctr, index_t *index, char **words);
void counters_intersection(counters_t *intersection, counters_t *ctr);
void counters_intsersection_helper(void *arg, const int key, const int count);
void add_firstword_counter(void *arg, const int key, const int count);
char ** read_input();


typedef struct counters_group {
	counters_t *firstCounter;
	counters_t *secondCounter;
} counters_group_t;

int
main(int argc, char *argv[])
{
	// if (argc == 3) {
	// 	//directory produced by crawler
	// 	char *directory = (char *)malloc((strlen(argv[1])+1)*sizeof(char));
	// 	strcpy(directory, argv[1]);

	// 	//file produced by indexer
	// 	char *indexFilename = (char *)malloc((strlen(argv[2])+1)*sizeof(char));
	// 	strcpy(indexFilename, argv[2]);

	// 	int lines;
	// 	int sizeOfTable;

	// 	//also checks if indexFilename exists
	// 	if ((lines = get_num_lines(indexFilename)) != -1) {
	// 		//determines the size of the hashtable based on words in file
	// 		if (lines >= 1000) {
	// 			sizeOfTable = (int) (lines/10);
	// 		} else {
	// 			sizeOfTable = lines;
	// 		}

	// 	} else {
	// 		fprintf(stderr, "The file %s did not exist\n", indexFilename);
	// 		free(directory);
	// 		free(indexFilename);
	// 		return 1;
	// 	}

	// 	//checks if the directory was produced by crawler
	// 	if (isCrawlerDirectory(directory)) {
	// 		index_t *index = index_new(250);
	// 		index_build(directory, index);
	// 		index_save(indexFilename, index);
	// 		index_delete(index);
	// 	} else {
	// 		free(directory);
	// 		free(indexFilename);
	// 		return 1;
	// 	}

	// 	//load the index from indexFilename into an internal data structure, with number of slots in hashtable
	// 	//dependent on number of words in file
	// 	index_t *index = index_new(sizeOfTable);
	// 	index_load(indexFilename, index);

	// 	query();
	// 	// counters_t *ctr;
	// 	// if ((ctr = index_find(index, test)) != NULL) {
	// 	// 	counters_iterate(ctr, stdout, print_count_helper);
	// 	// }

		

	// } else {
	// 	//too few or too many arguments
	// 	fprintf(stderr, "usage: ./querier pageDirectory indexFilename");
	// 	return 1;
	// }
	counters_t *intersection = counters_new();
	counters_set(intersection, 4, 3);
	counters_set(intersection, 5, 1);
	counters_set(intersection, 7, 6);

	counters_t *counter2 = counters_new();
	counters_set(counter2, 5, 2);
	counters_set(counter2, 6, 1);
	counters_set(counter2, 4, 2);

	counters_intersection(intersection, counter2);
	int check = counters_get(intersection, 5);
	printf("%d", check);

	//query();
}

void
query()
{
	char *line = freadlinep(stdin);
	line = NormalizeWord(line);

	char delim[] = " ";
	char *words[60]; // max number of commands
	char *ptr = strtok(line, delim);
	int j = 0;

	while(ptr != NULL)
	{	
		words[j] = ptr;
		ptr = strtok(NULL, delim);
		j++;
	}

	int i = 0;
	while (words[i] != NULL)
	{
		printf("%s\n", words[i]);
		i++;
	}
	//return words;
	//calculate_score(words);
	
	
}

void 
calculate_score(char **words)
{
	bool success = true;
	//int score = 0;
	if ((strcmp(words[0], "and") == 0) || (strcmp(words[0], "or") == 0)) {
		success = false;
	} 

	int i = 0;

	//keeps track of the scores of all the documents overall
	counters_t *documentScores = counters_new();

	//only continue as long as there are more words and no errors
	while ((words[i] != NULL) && (success == true))
	{
		//confirms that "and" & "or" are not adjacent
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
		//if the next word is or
		if (strcmp(words[i+1], "or") == 0) {
			//evaluate and-sequence

		}
		//else
			//add word to and sequence (concatentenate word + space)



		i++;
		
	}
}

void
evaluate_and_sequence(char *andsequence, index_t *index, counters_t *ctr)
{
	char delim[] = " ";
	char *andwords[60]; // max number of commands
	char *ptr = strtok(andsequence, delim);
	int j = 0;

	while(ptr != NULL)
	{	
		andwords[j] = ptr;
		ptr = strtok(NULL, delim);
		j++;
	}

	//counter to handle the values in the andSequence
	counters_t *andsequenceCtr = counters_new();

	
	if (andwords[0] != NULL) {
		//copies first word's counter to overall andsequencectr (intersection holder)
		counters_iterate(index_find(index, andwords[0]), andsequenceCtr, add_firstword_counter);
	

		//start after the first word
		int i = 1;
		while (andwords[i] != NULL)
		{
			counters_t *tempCtr = index_find(index, andwords[i]);
			counters_iterate(andsequenceCtr, tempCtr, counters_compare_helper);

		}
	}
}

void
add_firstword_counter(void *arg, const int key, const int count)
{
	counters_t *ctr = arg;
	counters_set(ctr, key, count);
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

