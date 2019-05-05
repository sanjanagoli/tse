# DESIGN - Tiny Search Engine, Indexer (Lab 5)

## User Interface

	Indexer's and indextest's only interface with the user is on the command-line; both must always have three arguments.

	`./indexer pageDirectory indexFilename`

	For example: `./indexer ../testDir testFile

	`./indextest oldIndexFilename newIndexFilename`

	For example: `./indextest testFile newTestFile

## Inputs and Outputs

### indexer.c

	Input: the only inputs are command-line parameters; see the User Interface above. Input is from pageDirectory that is created by `crawler.c`

	Output: Look through the directory of webpages that have been saved in this format `directory/pageId`. Examine the HTML to find all the words in the HTML and print the words to `indexFilename` in following format: word pageId count

### indextest.c
	
	Input: the only inputs are command-line parameters; see the User Interface above.

	Output: `newIndexFilename` contains all the words that are passed in from `oldIndexFilename` in a different order. 


## Functional decomposition into modules

Anticipating the following modules or functions:

1. *main*, which parses arguments and initializes other modules
2. *index_build*, which parses the HTML and places words into data structure
3. *index_save*, which prints the words line by line with corresponding *pageId* and count
4. *index_load*, loops through output from *index_save* to build data structure (*index*)
5. *NormalizeWord*, converts word to lowercase to standardize and account for repeated words that are different capitalization

And some helper modules that provide data structures:
*index* utilizes *hashtable* and *counters* to store word and corresponding pageId & count


## Pseudo code for logic/algorithmic flow

### indexer.c

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules --> check that directory comes from crawler
3. loop through all files in the directory, read in HTML, and parse to separate words
4. normalize the word (convert to lowercase) --> duplicates with different capitalization should not be counted twice
5. insert word into created data structure for later access
6. loop through all the words and place them in input file line by line, with corresponding pageId and count

### indextest.c

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules --> assumption: file comes from indexer.c output
3. determine the number of words/lines in the input file -- create hashtable size accordingly
4. read words and loop through all pageId and count pairs; insert in data structure
5. loop through all the words in data structure and place them in file line by line, with corresponding pageId and count

## Dataflow through modules

### indexer.c 
1. *main* parses parameters and passes them to indexer, checks if directory is from crawler
2. *index_build* will insert all the words from the file into the data structure *index*
3. *index_save* will print the words stored in *index* out to a file, line by line, with their corresponding pageId and count

### indextest.c
1. *main* parses parameters and passes them to indexer
2. *index_load* will take the output file from indexer.c and insert words into the data structure *index*
3. *index_save* will print the words stored in *index* out to a file, line by line, with their corresponding pageId and count

## Major data structures
1. *index* uses *hashtable* and *counters* to store words
2. *set* of words and *counters* (indirectly used by *hashtable*)
3. *counters* store the pageId and corresponding wordcount 
4. *hashtable* of words and *counters

## Testing Plan

1. Test the programs with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test the indexer with a file that was not created by crawler directory

3. Test the indexer with data directory provided and several other crawler prgraom outputs

4. Test the indextester with nonwritable newIndexFilename

