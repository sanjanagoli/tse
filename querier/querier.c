/*
* Sanjana Goli, April 2019
* Lab 6 (Tiny Search Engine) - querier.c
*
* Input: crawlerDirectory (directory with output produced by crawler program)
*		 indexFile (list of the words stored in the index structure from the webpages saved by crawlerDirectory)
*
* Output: prints out documents/scores that matched words from the query based on 'or' and 'and' combinations
*
* Description: Last component of the search engine; used to query the index file and ranks the documents based on which
*			   are most relevant to the query based on most occurences with the given combination
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
typedef struct counters_group counters_group_t;

void query(index_t *index, char* directory);
counters_t* calculate_score(char **words, index_t *index, int numWords);
counters_t * evaluate_and_sequence(char **andwords, index_t *index, int numAndWords);
counters_group_t * update_document_scores(counters_t *documentCounter, counters_t *andsequencectr);
void update_helper(void *arg, const int key, const int count);
counters_group_t * counters_intersection(counters_t *intersection, counters_t *ctr);
void counters_intsersection_helper(void *arg, const int key, const int count);
void add_firstword_counter(void *arg, const int key, const int count);
docscore_t ** sort_matches(counters_t *ctr);
void itemcount(void *arg, const int key, const int count);
void insert_array_helper(void *arg, const int key, const int count);
void delete_array(docscore_t **sortedArray, int numItems);
void display_results(char *query, int matches, char *crawlerDirectory, docscore_t **sortedArray);


/*counters_group: stores two counters in order to pass into counters_iterate in counters_intersection*/
typedef struct counters_group {
	counters_t *firstCounter;
	counters_t *secondCounter;
} counters_group_t;

/*docscore: used to passed into array easily for sorting and displaying to console*/
typedef struct docscore {
	int docId;
	int score;
	
} docscore_t;

/*main: parses parameters, ensures that command-line input is valid 
		(crawlerDirectory must be produced by crawler program, indexFile must exist*/
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
			fprintf(stderr, "Error: The file %s did not exist\n", indexFilename);

			free(directory);
			free(indexFilename);
			return 1;
		}

		//checks if the directory was produced by crawler, if not -> exit
		if (!isCrawlerDirectory(directory)) {
			free(directory);
			free(indexFilename);
			return 1;
		}


		//load the index from indexFilename into an internal data structure, with number of slots in hashtable
		//dependent on number of words in file
		index_t *index = index_new(sizeOfTable);
		index_load(indexFilename, index);

		//takes in inputs in order to correctly print and find words in index
		query(index, directory);

		index_delete(index);
		free(directory);
		free(indexFilename);

	} else {
		//too few or too many arguments
		fprintf(stderr, "usage: ./querier pageDirectory indexFilename");
		return 1;
	}
}

/* query: input - takes in index in order to pass into calculate score and evaluate_and_sequence (to determine)
		 if index contains word); takes in directory for printing later
		 reads in user input until EOF, tokenizes query, checks whether words contain bad characters,
		 and calls calculate score and  contains main functionality for program */

void
query(index_t *index, char* directory)
{	
	char *input;
	counters_t *documentScores;
	docscore_t **sortedArray;
	//reads until user ^D
	while( (input = freadlinep(stdin)) != NULL)   {
		bool error = false;
		char* line;

		//converts to lowercase
		line = NormalizeWord(input);

		int numberWords = 0;
		char delim[] = " ";
		
		//copies in order to find how many words are in query without affecting input
		char *linecopy = malloc((strlen(line)+1)*sizeof(char));
		strcpy(linecopy, line);
		char *count = strtok(linecopy, delim);

		//determines how many words are in the query in order to correctly allocate array
		while(count != NULL)
		{	
			count = strtok(NULL, delim);
			numberWords++;
		}
		free(linecopy);

		//tokenizes each of the words to input into array
		char *ptr = strtok(line, delim);
		int j = 0;

		char *words[numberWords]; 
		while(ptr != NULL)
		{	
			words[j] = ptr;
			ptr = strtok(NULL, delim);
			j++;
		}

		//checks if there is any bad character in query by looping through andchecking each character
		int numWords = j;
		int l = 0;
		while (l < j) {
			for (int k = 0; k < strlen(words[l]); k++) {
				if( !isalpha(words[l][k]) && !isspace(words[l][k]) ) {
					fprintf(stderr, "Error: bad character '%c' in query\n", words[l][k]);
					error = true;
				}

			}
			l++;
		}

		/*checks to see if 'and' and 'or' are adjacent
		  checks if 'and' or 'or' is the last words in the query
		  checks if 'and' or 'or' is the first word in query*/
		for (int i = 0; i < numWords; i++) {
			if (i == 0) {
				//checks current word and adjacent word
				if ( (strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0) ) {
					fprintf(stderr, "Error: '%s' cannot be first word of query\n", words[i]);
					error = true;
				}
			} else {
				//if the word is not the last word in the query
				if (i < numWords-1) {
					if ( (strcmp(words[i], "and") == 0) && ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) ) {
						fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
						error = true;
					
					}
					if ( (strcmp(words[i], "or") == 0) && ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) ) {
						fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
						error = true;
					}
				}
				
				if (i == numWords-1) {
					if ( (strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0) ) {
						fprintf(stderr, "Error: %s cannot be at the end of the query!\n", words[i]);
						error = true;
					}
				}
			}
		}

		//if there is no error with input, then proceed
		if (!error) {

			documentScores = calculate_score(words, index, numWords);
			sortedArray = sort_matches(documentScores);
			int numItems = 0;
			counters_iterate(documentScores, &numItems, itemcount);

			//how many results have matched/to display based on how many scores are 0
			int nonZeroCount = 0;
		    for (int i = 0; i < numItems; i++) {
		    	if (sortedArray[i]->score > 0) {
		    		nonZeroCount++;
		    	}
		    }

		    //prints matches/documents/scores to console
		    display_results(input, nonZeroCount, directory, sortedArray);
		    
			delete_array(sortedArray, numItems);
			counters_delete(documentScores);
		}
		free(input);
		free(line);
	}
}

/* calculate_score: input - words and numWords are needed to iterate through query
		loops through each of the words to determine whether the word is an 'or' or 'and' 
		if the next word is 'or', evaluate the words that have already been read in and reset
		andsequence array; if there are no more words, evaluate sequence as well */
counters_t * 
calculate_score(char **words, index_t *index, int numWords)
{
	bool success = true;
	if ((strcmp(words[0], "and") == 0) || (strcmp(words[0], "or") == 0)) {
		fprintf(stderr, "'%s' cannot be the first word\n", words[0]);
		success = false;
	} 

	int i = 0;

	//keeps track of the scores of all the documents overall
	counters_t *documentScores = counters_new();

	char *andsequence[100];
	int j = 0;
	//only continue as long as there are more words and no errors
	while ((i < numWords) && (success)) {
		//confirms that "and" & "or" are not adjacent
		if (i < numWords-1) {
			if ( (strcmp(words[i], "and") == 0) && ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) ) {
				fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
				success = false;
			
			}
			if ( (strcmp(words[i], "or") == 0) && ( (strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0) ) ) {
				fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
				success = false;
				break;
			}
		}
		
		//if the last word is an 'and' or 'or'
		if (i == numWords-1) {
			if ( (strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0) ) {
				fprintf(stderr, "Error: %s cannot be at the end of the query !", words[i]);
				success = false;
				break;
			}
		}

		//if the next word is or
		if ((i < numWords-1) && (strcmp(words[i+1], "or") == 0) && (success)) {
			//evaluate and-sequence after adding current word
			andsequence[j] = words[i];
			counters_t *tempCtr = evaluate_and_sequence(andsequence, index, j+1);
			
			//after evaluating sequence, update total counter that stores the collected scores so far
			counters_group_t *group = update_document_scores(documentScores, tempCtr);
			counters_delete(tempCtr);
			free(group);

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
			//add word to and sequence array if the next word is not or
			andsequence[j] = words[i];
			if (i+1 == numWords) {
				//if there are no more words in the sequence
				counters_t *tempCtr = evaluate_and_sequence(andsequence, index, j+1);
				counters_group_t *group = update_document_scores(documentScores, tempCtr);
				counters_delete(tempCtr);
				free(group);
			}
			j++;
		}
		i++;
		
	}
	return documentScores;
}

/*evaluate_and_sequence: input - takes in words from andsequence and numWords in order to loop through
		if the word exists in the index, add its counter to the total counter based on 'and' 'or' intersection logic */
counters_t *
evaluate_and_sequence(char **andwords, index_t *index, int numAndWords)
{
	//counter to handle the values in the andSequence
	counters_t *andsequenceCtr = counters_new();

	//if there first word is not null (don't perform actions otherwise)
	if (andwords[0] != NULL) {
		//copies first word's counter to overall andsequencectr (intersection holder)
		counters_t *wordCounter = index_find(index, andwords[0]);

		//if the word is not in the index, don't iterate
		if (wordCounter != NULL) {
			counters_iterate(wordCounter, andsequenceCtr, add_firstword_counter);
		}
		

		//start after the first word
		int i = 1;
		while (i < numAndWords) {
			//find the intersection of all the other words with the first word
			counters_t *tempCtr = index_find(index, andwords[i]);
				//updates total andsequenceCtr with intersection information
				counters_group_t * group = counters_intersection(andsequenceCtr, tempCtr);
				free(group);
			i++;
		}
	}
	return andsequenceCtr;
}

/*add_firstword_counter: helper method for counters_iterate to copy counter of first word to new counter*/
void
add_firstword_counter(void *arg, const int key, const int count)
{
	counters_t *ctr = arg;
	counters_set(ctr, key, count);
}

/*update_document_scores: input: updates the total counter of query based on minimum values (if and) 
	and total values (if or); returns counters_group_t for free memory above*/
counters_group_t *
update_document_scores(counters_t *documentCounter, counters_t *andsequencectr)
{
	//need to keep track of two counters
	counters_group_t *group = malloc(sizeof(counters_group_t));

	//first counter contains the single sequence and document counter keeps track of whole thing
	group->firstCounter = andsequencectr;
	group->secondCounter = documentCounter;
	counters_iterate(andsequencectr, group, update_helper);
	return group;
}

/* update_helper: used to help counters_iterate update intersection counter with minimum score*/
void
update_helper(void *arg, const int key, const int count)
{

	counters_group_t *group = arg;
	counters_set(group->secondCounter, key, counters_get(group->secondCounter, key) + counters_get(group->firstCounter, key));
}

/*counters_intersection: contains large functionality of program
	input - two counters
	output - returns the intersection of the counter and favors minimum count
	used in evaluate_and_sequence method */

counters_group_t *
counters_intersection(counters_t *intersection, counters_t *ctr)
{
	counters_group_t *group = malloc(sizeof(counters_group_t));

	//one counter holds the passed in values and one holds the intersection of all counters passed into it
	group->firstCounter = ctr;
	group->secondCounter = intersection;

	//updates intersection counter based on specifications
	counters_iterate(intersection, group, counters_intsersection_helper);
	return group;
}

/* counters_intersection_helper: used in counters_iterate in counters_intersection
	sets one counter in counter group with minimum value (between counters) -> 0 if one counter does 
	not contain the key
*/
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

/*	sort_matches: takes the total counter for the query and inputs elements into an array
	sorts the array with nested loops 
	returns array to allow for freeing allocated memory above
*/
docscore_t **
sort_matches(counters_t *ctr)
{
	//determine the number of elements in the counter
	int numItems = 0;
	counters_iterate(ctr, &numItems, itemcount);
	docscore_t **sortedArray = calloc(numItems, sizeof(docscore_t*));

	counters_iterate(ctr, sortedArray, insert_array_helper);

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
	return sortedArray;
}

/*insert_array_helper: passed into counters_iterate above in order to help insert each element into the array
	uses while loop to determine where to insert (if the slot in the array is not full, insert there*/
void 
insert_array_helper(void *arg, const int key, const int count)
{
	docscore_t **sortedArray = arg;
	//need to allocate each of the docscore structs even though the array is allocated
	docscore_t *docscore = malloc(sizeof(docscore_t));
	docscore->docId = key;
	docscore->score = count;

	//insert the docscore struct at the first place where the slot is not full
	//this works because there is already a number of elements calloc'd for the array
	int i = 0;
	while (sortedArray[i] != NULL) {
		i++;
	}
	sortedArray[i] = docscore;
}

/* display_results: prints to console in format specified by the spec */
void
display_results(char *query, int matches, char *crawlerDirectory, docscore_t **sortedArray)
{
	printf("Query: %s\n", query);
	printf("Matches %d documents\n", matches);
	for (int i = 0; i < matches; i++) {
		//index method used in order to determine location of crawler output
		char *filename = createFilename(sortedArray[i]->docId, crawlerDirectory);
		FILE *fp;
		//accesses the url based on the docId from the crawler output
		if ((fp = fopen(filename, "r")) != NULL) {
			char *url = freadlinep(fp);
			printf("score %3d   doc %3d: %s\n", sortedArray[i]->score, sortedArray[i]->docId, url);
			free(url);
		}
		fclose(fp);
		free(filename);
	}
	printf("--------------------------------------------------------\n");

}

/*delete_array: method used to help free all the memory allocated to the array*/
void
delete_array(docscore_t **sortedArray, int numItems)
{
	
	for(int i = 0; i < numItems; i++) {
		free(sortedArray[i]);
	}
	free(sortedArray);
}

/*itemcount: helper method used for counters_iterate to determine how many elements are in the counter*/
void 
itemcount(void *arg, const int key, const int count)
{
  int *nitems = arg;

  if (nitems != NULL && (key >= 0) && (count >= 0))
    (*nitems)++;
}



