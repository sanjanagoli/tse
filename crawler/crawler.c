/*
* Sanjana Goli, April 2019
* Lab 4 - crawler.c
* 
* Input: The user must input 
*	1. seedURL (must be internal) 
*	2. pathname (of an existing directory) 
*	3. maxdepth (between 1 and 10) - represents minimum crawl depth
* 
* Output:
*   File in specified directory with following contents:
*		- url
*		- depth
*		- html
*	Progress indicators that show the caller if the URL was fetched, scanned, found, etc.
*
* Description: Program crawls the web starting at the specified URL and searches through the page's next URLs
			   based on specified depth. Needs to check if the URL is valid (internal) and searches accordingly. 
* 			   Part 1 of 3 for Tiny Search Engine
*			   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

void crawler(char *url, char *pathname, int maxDepth);
bool pagefetcher(webpage_t *page);
void pagescanner(webpage_t *page, hashtable_t *seenUrls, bag_t *toCrawl);
inline static void logr(const char *word, const int depth, const char *url);
void deletePage(void *item);
void deleteURL(void *item);


int 
main(int argc, char *argv[])
{
	if (argc == 4) {

		//converts input to integer
		int maxDepth = atoi(argv[3]);

		//need to make copy of arguments (can't use them directly)
		char *filename = (char *)malloc(10+(strlen(argv[2]))*sizeof(char));
		strcpy(filename, argv[2]);
		strcat(filename, "/.crawler");
		
		

		//check if directory exists
		if (check_directory(filename) == false)
		{
			//need to free filename because it is not being used
			free(filename); 
			return 1;
		}

		char *pathname = (char *)malloc((1+strlen(argv[2]))*sizeof(char));
		strcpy(pathname, argv[2]);


		char *url = (char *)malloc((1+strlen(argv[1]))*sizeof(char));
		strcpy(url, argv[1]);

		//have to normalize url before checking if it is internal below
		NormalizeURL(url);

		//checks if input is valid
		if (IsInternalURL(url) == false|| !(maxDepth >= 0 && maxDepth <= 10)) {
			fprintf(stderr, "URL must be an internal URL and maxdepth must be between 1-10. \n");

			//have to free because not used
			free(url);
			free(pathname);
			free(filename);
			return 1;
		}

		crawler(url, pathname, maxDepth);

		//after crawler is invoked free memory accordingly
		free(pathname);
		free(filename);


	//is not valid usage of program
	} else {
		fprintf(stderr, "usage: ./crawler seedURL pageDirectory maxDepth \n");
		return 1;
	}
}

/*****************crawler**************************/
/* Performs the primary functionality of the program:
*	1. Extracts a webpage from the bag (continue until there are no more pages)
*	2. Use savepage from pagedir.c to store the HTML for the webpage
*	3. If the depth of the page is less than the maxDepth, invoke pagescanner
*	4. At the end of the loop, free all the variables that were allocated through malloc/calloc
*/

void
crawler(char *url, char *pathname, int maxDepth)
{	
	//initializing modules to be used in program
	hashtable_t *seenUrls = hashtable_new(100);
	bag_t *toCrawl = bag_new();
	
	if (seenUrls == NULL) {
	    fprintf(stderr, "hashtable was not properly initialized \n");
  	} 
 	if (toCrawl == NULL) {
  		fprintf(stderr, "bag was not propertly initialized \n");
  	}


	webpage_t *seed = webpage_new(url, 0, NULL);
	if(seed == NULL) {
		fprintf(stderr, "Error: seed is null");
	}

	//insert to the initial into the bag and hashtable to extract below
	bag_insert(toCrawl, seed);
	hashtable_insert(seenUrls, url, " ");

	int id = 1;
	webpage_t *page;
	while ( (page = bag_extract(toCrawl)) != NULL ) {
		if (pagefetcher(page)) {
		
			savepage(page, id, pathname);

			//need to increment after saving page so that each page has unique id in directory
			id++;

			//if there is still more depth to search continue scanning
			if (webpage_getDepth(page) < maxDepth) {
				pagescanner(page, seenUrls, toCrawl);
			} 
		}
		//need to free up allocated webpage
		webpage_delete(page);

	}

	//need to free up space allocated to bag and hashtable
	bag_delete(toCrawl, deletePage);
	hashtable_delete(seenUrls, deleteURL);
	

}

/*****************pagefetcher**************************/
/* Pagefetcher uses the webpage_fetch method from webpage.c
*  If the webpage has not been fetched, delete the allocated variables
*  If the webpage has been fetched, save and scan the page, then delete
*/

bool
pagefetcher(webpage_t *page)
{

	if(webpage_fetch(page) == false) {
		fprintf(stderr, "The webpage was not fetched. \n");
		return false;
	} else {
		logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
		return true;
	}	
}

/********************pagescanner**************************/
/* Pagefetcher uses the webpage_getNextURL method from webpage.c
*  As long as the bag has more webpages to scan, continue searching 
*  through the page to find URLs that link to other pages and add to bag and hashtable
*  Caller: only call if depth of page is less than maxDepth (passed in)
*/

void
pagescanner(webpage_t *page, hashtable_t *seenUrls, bag_t *toCrawl)
{
	logr("Scanning", webpage_getDepth(page), webpage_getURL(page));

	//obtaining each URL in the webpage
	int pos = 0;
	char *result;

	if(page != NULL) {
		//as long as there are more URLs that link to other pages
		while ((result = webpage_getNextURL(page, &pos)) != NULL) {
			logr("Found", webpage_getDepth(page), result);
			if (IsInternalURL(result)) {
				if (hashtable_insert(seenUrls, result, " ")) {
					//need to insert each page to bag in order to search later
					webpage_t *nextPage = webpage_new(result, webpage_getDepth(page)+1, NULL);
					bag_insert(toCrawl, nextPage);
				} else {
					logr("IgnDupl", webpage_getDepth(page), result);

					//need to free the result if url is not being used for a webpage
					free(result);
				}
			} else {
				logr("IgnExtrn", webpage_getDepth(page), result);

				//need to free the result if url is not being used for a webpage
				free(result);
			}
  		}
	}
}


/*logr: prints status updates to the caller*/
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %s%9s: %s\n", depth, "", word, url);
}

/* deletePage: method to delete remaining items (shouldn't be any) from bag) */
void deletePage(void *item)
{
  if (item != NULL) {
    webpage_delete(item);
  }
}

/* deleteURL: need empty method to handle the empty string items passed into hashtable's value
   do not need to free the string because it was not allocated with malloc/calloc */
void deleteURL(void *item) {}


