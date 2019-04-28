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
*
* Description: Program crawls the web starting at the specified URL and searches through the page's next URLs 
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

		free(pathname);
		free(filename);


	//is not valid usage of program
	} else {
		fprintf(stderr, "usage: ./crawler seedURL pageDirectory maxDepth \n");
		return 1;
	}
}

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


	bag_insert(toCrawl, seed);
	hashtable_insert(seenUrls, url, " ");

	int id = 1;
	webpage_t *page;
	while ( (page = bag_extract(toCrawl)) != NULL ) {
		if (pagefetcher(page)) {
		
			savepage(page, id, pathname);

			//need to increment after saving page so that each page has unique id in directory
			id++;


			if (webpage_getDepth(page) < maxDepth) {
				pagescanner(page, seenUrls, toCrawl);
			} 
		}
		webpage_delete(page);

	}

	//need to free up space allocated to bag and hashtable
	bag_delete(toCrawl, deletePage);
	hashtable_delete(seenUrls, deleteURL);
	

}

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


void
pagescanner(webpage_t *page, hashtable_t *seenUrls, bag_t *toCrawl)
{
	logr("Scanning", webpage_getDepth(page), webpage_getURL(page));

	int pos = 0;
	char *result;

	if(page != NULL) {
		while ((result = webpage_getNextURL(page, &pos)) != NULL) {
			
			//NormalizeURL(result);
			logr("Found", webpage_getDepth(page), result);
			if (IsInternalURL(result)) {
				if (hashtable_insert(seenUrls, result, " ")) {
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


