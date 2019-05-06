# Implementation - Tiny Search Engine, Indexer (Lab 5)

## Detailed pseudo code for each of the objects/components/functions

### indexer.c

1. Take in input from command line and create strings for `directory` and `indexFilename`
2. Check if the directory was produced by the crawler program
    a. Create new string that concatenates the directory name with "./crawler"
    b. Open the file if possible, if not then exit
3. Initialize `index_t` module with number of hashtable slots between 200 and 900
4. Run `index_build`, which takes in the directory name and `index` 
    a. create the filename based on directory name and pageId
    b. read in the `url`, `depth`, and `html` and create a `webpage` with these values
    c. while there are words in the html
        1. use `NormalizeWord` on each word (convert to lowercase)
        2. check whether the string length is greater than 3
    d. clean up variables (free allocated memory) and updated name of file
5. Run `index_save`, which takes in file to print out words and `index`
    a. Create/open a file of specified name
    b. iterate through each of the each of the elements/words in `hashtable`
    c. each `set` contains *key, value* pairs in following format: (word, `counters`) --> each `counter` contained *pageId* and *wordcount* 
    d. print each of the words and counts to the file in the following format: word pageId count pageId count


### indextest.c

1. Take in input from command line and create strings for `oldIndexFilename` and `newIndexFilename`
2. Retrieve number of lines in `oldIndexFilename` using `get_num_lines` in `index.c` and initalize new `index` module
3. Run `index_load`, which takes in `oldIndexFilename` and `index`
    a. Reads in word on every line
    b. loops through integers to collect *key, value* pairs for *pageId, count*
    c. while there are more lines in `oldIndexFilename`, call `index_set_wordcount` to specify the pairing of (*word*, (*pageId, count*))
4. Run `index_save`, which takes in file to print out words and `index`
    a. Create/open a file of specified name
    b. iterate through each of the each of the elements/words in `hashtable`
    c. each `set` contains *key, value* pairs in following format: (word, `counters`) --> each `counter` contained *pageId* and *wordcount* 
    d. print each of the words and counts to the file in the following format: word pageId count pageId count

    

## Function prototypes and their parameters

### index.c
```c
typedef struct index index_t;
index_t * index_new(int num_slots);
void index_insert_word(index_t *index, const char *word, int pageId);
void index_set_wordcount(index_t *index, const char *word, int pageId, int countVal);
void index_save(char *filename, index_t *index);
int get_num_lines(char *filename);
void print_word_counter(void *arg, const char *key, void *item);
void print_count_helper(void *arg, const int key, const int count);
void index_load(char *filename, index_t *index);
void index_delete(index_t *index);
void delete_helper(void *item);
void index_build(char *directory, index_t *index);
char * createFilename(int id, char *dirname);
```
`index_new`: takes in number of slots to initailize the `hashtable`. `num_slots` depends on whether the caller knows how many words/keys are going to be inputted; `hashtable` contains several `sets` with *keys* that are words and values that are `counters`. 

`index_insert_word`: takes in *word* and *pageId* to confirm whether they are valid inputs (*pageId* > 0); if the word does not exist, create a `counter` for the word and insert into `hashtable`; to be called in `index_build`

`index_set_wordcount`: takes in *word* and *pageId* to confirm whether they are valid inputs (*pageId* > 0); if the word does not exist, create a `counter` for the *word* and insert into `hashtable` with key and value combination; to be called in `index_load`

`index_save`: create new writable file and print word, pageId, and count in format specfied below

`get_num_lines`: call `lines_in_file` from `file.c` in order to determine the size of the `hashtable` in `index` data structure

`index_load`: loads in words from input file to `index` data structure, calls` index_set_wordcount`

`index_delete`: frees memory allocated to the data structure --> frees `index` itself and `hashtable` of data structure

`index_build`: scans file that is inputted, creates a `webpage` from *url* and *html* given, parses *html* to find words,
inserts words into `index` using `index_insert_word`

`createFilename`: creates filename of format dirname/id free(filename)

### pagedir.c

`bool isCrawlerDirectory(char *directory)`: method checks whether the directory given was produced by `crawler`

### word.c

`char * NormalizeWord(char *word)`: returns a lowercased version of the *word* passed in


## Data structures

`index`:
An `index` contains a `hashtable`. The *key* of the `hashtable` is a *word* and the *value* is a `counters` with *key* as pageId and *value* as count

`hashtable`: 
A `hashtable` is maintains sets of (_key_, _item_) pairs. _Keys_ are unique (can only appear once in a `set`). 
Each `set` in the `hashtable` starts empty, grows as the caller adds one (_key_, _item_) pair at a time. 
It could be empty, or could contain hundreds of (_key_, _item_) pairs.
_Keys_ are unique, and can be specifically referred to in methods like insert and find

`webpage`:

This module defines the opaque `webpage_t` type, and a suite of functions for creating and manipulating "web pages".  A "web page" is really a struct holding 

* the URL of this web page, in canonical form
* the depth at which the crawler found this page
* the HTML for the page - may be NULL. 

`counters`:

A `counters set` is a set of counters, each distinguished by an integer _key_, with each key only occurring once in the set
The `counter` starts empty, grows as the caller adds one _key_ at a time
It could be empty, or could contain hundreds of _keys_.
_Keys_ are unique, and can be specifically referred to in methods like add, get, and set

## Error handling and recovery

1. Test the programs with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test the indexer with a file that is not writable

3. Test the `indexer` with data directory provided and several other `crawler` program outputs

4. Test the `indexer` with a directory that was not created by crawler program

5. Test the `indextest` with nonwritable `newIndexFilename`

6. Test the `indextest` with an oldIndexFile that does not exist

7. Test an actual application of program wiht output from `indexer` program

8. Confirm success of program by sorting input and output files and comparing results

9. Freed any allocated memory for all of these boundary cases listed above.
