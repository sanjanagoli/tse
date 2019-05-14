# DESIGN - Tiny Search Engine, Querier (Lab 6)

## User Interface

Querier takes in command-line arguments in the following format:

`./querier crawlerDirectory indexFilename`  
For example: `./querier ../crawler/testDir indexFile`  


Then, if all inputs are validated, the program will prompt the user to enter input to `stdin`
	
For example: `DARTMOUTH college or computer SCIENCE`  

Alternatively, the caller can pass in arguments in the command-line call in the following format:
	
`./querier crawlerDirectory indexFilename < tester` where `tester` contains various test cases OR  
`echo dartmouth college or computer science | ./querier crawlerDirectory indexFilename`

## Inputs and Outputs

Inputs: inputs are taken in the format discussed above; the user is prompted for standard input after the command-line arguments are verified, or they can pass in standard input during the command-line call. *index* structure in program uses input from *indexFilename*

Output: Produces a list of documents, corresponding scores, and document urls that are listed in ascending order by scores. Does not ouptut document/scores that have not matched - prints "Matches 0 documents".


## Functional decomposition into modules

Anticipating the following modules or functions (functions that contain functionality - other helper methods will be used in program):

1. structure *docscore* will hold documentId and scores
2. structure *counters_group* will hold two counters 

3. *query*, will hold the main functionality of the program, calling *calculate_scores*, *sort_matches*, and *display_results*
4. *calculate_score*, will loop through query and determine when to calculate the score for the sequence of words depending on 'and' and 'or' operators
5. *evaluate_and_sequence*, evaluate the specific and sequence passed into method
6. *update_document_scores*, updates the total scores of the query held in *calculate_score*
7. *counters_intersection*, finds the intersection of the counters and prefers minimum count given two counters
8. *sort_matches*, given *key, value* pairs, sort by ascending order based on count
9. *display_results*, displays results to the screen based on specifications on Querier lab
10. *main*, validates input and calls *query* method that calls other methods within methods


## Pseudo code for logic/algorithmic flow

1. execute from a command line as shown in the User Interface and valid command line arguments
2. take in standard input and pass into *query* 
	- will tokenize each of the words to remove spaces and only insert words into array
	- *query* will error-check to make sure that the standard input is valid (based on spec)
3. calculate the document score using *calculate_score*
	- would take in words from query and loop through each word to incorporate 'and' 'or' operator logic
	- when the next word is 'or' or if it is the last word in the query --> evaluate the individual and-sequence
4. *evaluate_and_sequence* would find the intersection of the counters based on if the word exists in the index using *counters_intersection* and prefer the minimum count between the two counters
	-  *counters_intersection* iterates through one `counter` and checks if the other `counter` contains the key or not; replace minimum *count* with each 
	- *counters_intersection* takes in *counters_group* so that it has a group of two counters to deal with
5. Program updates the total scores of all the documents after the sequence is evaluated, using *update_document_scores*
6. Sort the matches after the document scores are collected

## Dataflow through modules

1. *query*, takes in *index* that is created in *main* through *index_load* and *directoryName* to pass into method later
2. *calculate_score* is called by *query* and takes in list of words that are passed in by stdin and tokenized by *query*; also takes in *index* to pass into *evaluate_and_sequence*
3. *evaluate_and_sequence*, takes in list of words in andsequence and *index* in order to find corresponding *counters* and add to total *documentScores* in *calculate_score*
4. *counters_intersection*, takes in two counters --> forms a *counters_group* with two counters to pass into `counters_iterate`
5. *update_document_scores*, takes in two counters and adds values from one counter to total counter
6. *sort_matches*, takes in counter and sorts values based on ascending order after inserting each element into array

## Major Data Structures

1. *docscore* will hold documentId and scores
2. *counters_group* will hold two counters 
3. *index* created by loading in information from *indexFile*
4. *counters* stores *docId, count* pairs

## Testing Plan

1. Test to make sure the program validates command lind input correctly
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

