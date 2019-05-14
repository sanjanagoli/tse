# Implementation - Tiny Search Engine, Querier (Lab 6)

## Detailed pseudo code for each of the objects/components/functions

1. execute from a command line as shown in the User Interface and valid command line arguments in `main`
	- use `isCrawlerDirectory` method from `index` to confirm that `directory` is produced by `crawler` program
2. take in standard input and pass into `query` method
	- will tokenize each of the words using `strtok` to remove spaces and only insert words into array
	- calculates number of words in input in order to properly allocated memory for the array
	- error-check to make sure that the standard input is valid (based on spec)
		- loop through each character in the query to confirm that there are only alphabetic or space characters
		- loop through each word to confirm that 'and' 'or' operators are only in valid locations (not at beginning, mutliple adjacent, or at the end)
	- if there are no errors detected, call `calculate_score`, which returns a `counters` set
3. calculate the document score using `calculate_score`
	- would take in words from query and loop through each word to incorporate 'and' 'or' operator logic
	- when the next word is 'or' or if it is the last word in the query --> calls `evaluate_and_sequence` at this point
4. `evaluate_and_sequence` would find the intersection of the counters based on if the word exists in the index using *counters_intersection* and prefer the minimum count between the two counters
	- `counters_intersection` iterates through one `counter` and checks if the other `counter` contains the key or not; replace minimum *count* with each 
	- `counters_intersection` takes in two `counters` so that it has a group of two `counters` to deal with
5. Program updates the total scores of all the documents after the sequence is evaluated, using `update_document_scores`
6. Call `sort_matches` to insert elements of `documentScores` into array and using sorting algorithm to sort by *count* of each document
	- allocate memory for the array based on the number of elements in the counter
	- loop through array until the slot is not null, and insert element in that position for sorting
7. Display the results to the console by calling `display_results`
	- reads the first line of the `crawler`-produced file in order to get *url*
	- prints number of matches based on non-zero count in array

## Function prototypes and their parameters
```c
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
```

`query`: input - takes in index in order to pass into calculate score and `evaluate_and_sequence` (to determine); if `index` contains word); takes in directory for printing later; reads in user input until EOF, tokenizes query, checks whether words contain bad characters, and calls calculate score and contains main functionality for program

`calculate_score`: input - words and numWords are needed to iterate through query loops through each of the words to determine whether the word is an 'or' or 'and'. if the next word is 'or', evaluate the words that have already been read in and reset andsequence array; if there are no more words, evaluate sequence as well

`evaluate_and_sequence`: input - takes in words from andsequence and numWords in order to loop through if the word exists in the index, add its counter to the total counter based on 'and' 'or' intersection logic

`add_firstword_counter`: helper method for `counters_iterate` to copy `counter` of first word to new `counter`

`update_document_scores`: input - updates the total `counter` of *query* based on minimum values (if and) and total values (if or); returns `counters_group_t` for freeing memory in above method

`counters_intersection`: contains large functionality of program; input - two counters; output - returns the intersection of the counter and favors minimum count after iterating through first `counter` and updating intersection `counter`; used in `evaluate_and_sequence` method

`counters_intersection_helper`: used in `counters_iterate` in `counters_intersection`; sets one `counter` in `counter_group` with minimum value (between `counters`) -> 0 if one `counter` does not contain the key

`sort_matches`: takes the total `counter` for the *query* and inputs elements into an array; sorts the array with nested loops; returns array to allow for freeing allocated memory above method

`display_results`: prints to console in format specified by the querier requirements spec

`delete_array`: method used to help free all the memory allocated to the array (in `sort_matches`)

## Data structures

`counters_group`:
A `counters_group` is a group of two `counters` that is used to find the intersecting points of `counters` and updating one of the `counters` in the group with the combined value (based on minimum if and or addition if or)

`counter`:
A `counter` holds a *key, value* pair, where the *key* and *values* are integers; *keys* cannot be repeated within the counter --> used to hold docId and corresponding count of word

`docscore`:
Holds the *docId* and the *count* --> essentially the same information as a counter, but formatted into one node

`index`:
An `index` contains a `hashtable`. The *key* of the `hashtable` is a *word* and the *value* is a `counters` with *key* as pageId and *value* as count

## Security and privacy properties

Program only performs the functions if the URL is internal (the URL stays within the CS50 playground http://old-www.cs.dartmouth.edu/)

## Error handling and recovery

Test to make sure the program validates command lind input correctly
	- test case that is not whether directory is produced by crawler
	- test case of a indexFile that does not exist

2. Test boundary cases for conjuction operators
	- operators at beginning
	- operators at end
	- operators adjacent

3. Test boundary cases with bad characters in the middle/end/beginning

4. Test case with capitalized word (to ensure the word is normalized)

5. Test case with word that does not exist in the file

6. Test queries with several spaces in different places

7. Test various combinations of operator usage, individually and combined


