# Implementation - Tiny Search Engine

## Detailed pseudo code for each of the objects/components/functions

	1. Initialize moduels - `bag`, `hashtable`, `webpage` (with `seedURL`)
	2. Extract a `webpage` from the `bag` (continue until no more pages)
	3. Use `pagefetcher` to fetch the HTML for the corresponding URL
		a. `pagefetcher` mainly uses the `webpage_fetch` method provided in `webpage.c`
		b. If the webpage was not fetched, return false; else, return true
		c. Print necessary message 
	4. Use `savepage` from `pagedir.c` to store the HTML for the webpage
		a. Creates the path for the according file in the following format: "pathname/id"
		b. Need to concatentate (using `strcat`) these parts after converting int to string
		c. Write to file using fprintf (URL, depth, html)
		d. free the filename that was allocated
	5. If the depth of page is less than the `maxDepth` (passed in), invoke `pagescanner`
		a. Calls `webpage_getNextURL` in a while loop --> continue looping as long as there are URLs on a page that connect to another internal page 
		b. Insert the URL into the `hashtable` and the `webpage` into the `bag`
		c. Free URL if it was not added to to the hashtable
	6. At the end of the loop, need to free the `webpage` that was allocated; after the loop, free the `bag` and `hashtable` passing in the `deletePage` and `deleteURL` functions respectively

## Function prototypes and their parameters

### crawler.c
```c
void crawler(char *url, char *pathname, int maxDepth);
bool pagefetcher(webpage_t *page);
void pagescanner(webpage_t *page, hashtable_t *seenUrls, bag_t *toCrawl);
inline static void logr(const char *word, const int depth, const char *url);
void deletePage(void *item);
void deleteURL(void *item);
```

`crawler`: Method is called in the `main` method after inputs have been initalized; includes `pagefetcher`, `savepage`, and `pagescanner` methods

`pagefetcher`: Use method to get the HTML for the specified URL, utlizes `webpage_fetch` from `webapge.c`

`pagescanner`: Use method to add more URLs and pages to the `hashtable` and `bag` respectively; will continue as long as more URLs exist on the `webpage`

`logr`: Use this method to print status updates

`deletePage`: passed into `bag_delete` to delete remaining items in bag if any

`deleteURL`: placeholder method (does not have any functionality)

### pagedir.c

```c
bool check_directory(char *filename);
void savepage(webpage_t *page, int id, char *pathname);
```

`check_directory`: Use to determine whether the directory exists; creates a new dotfil to place in the directory if it exists

`savepage`: If the directory exists, print the URL, page depth, and HTML to a file named by id and put in specified directory

## Data structures

`hashtable`: 
A `hashtable` is maintains sets of (_key_, _item_) pairs. _Keys_ are unique (can only appear once in a `set`). 
Each `set` in the `hashtable` starts empty, grows as the caller adds one (_key_, _item_) pair at a time. 
It could be empty, or could contain hundreds of (_key_, _item_) pairs.
_Keys_ are unique, and can be specifically referred to in methods like insert and find

`bag`:
A `bag` is an unordered collection of _items_.
The `bag` starts empty, grows as the caller adds one _item_ at a time, and shrinks as the caller extracts one _item_ at a time.
It could be empty, or could contain hundreds of _items_.
_Items_ are indistinguishable, so the _extract_ function is free to return any _item_ from the `bag`.

`webpage`:

This module defines the opaque `webpage_t` type, and a suite of functions for creating and manipulating "web pages".  A "web page" is really a struct holding 

 * the URL of this web page, in canonical form
 * the depth at which the crawler found this page
 * the HTML for the page - may be NULL. 

## Security and privacy properties

Program only performs the functions if the URL is internal (the URL stays within the CS50 playground `http://old-www.cs.dartmouth.edu/`)

## Error handling and recovery

1. Testing program handles for boundary cases including: 
	- invalid parameters (too low or high depth, nonexistent URL, non-internal URL)
	- specifically, if the URL points to a non-existent server or valid server but non-existent page
	-  if the user passes in too many arguments
2. Program frees any allocated information for the above boundary cases to ensure no memory leaks

