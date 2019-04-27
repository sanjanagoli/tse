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
* 
*
* Description: 
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
// #include "pagedir.h"

void crawler(char *url, char *pathname, int maxDepth);
bool pagefetcher(webpage_t *page);
void pagesaver(webpage_t *page, int id, char *pathname);
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


		char *pathname = (char *)malloc((1+strlen(argv[2]))*sizeof(char));

		strcpy(pathname, argv[2]);

		// char *pathname = argv[2];

		char *url = (char *)malloc((1+strlen(argv[1]))*sizeof(char));
		strcpy(url, argv[1]);

		//have to normalize url before checking if it is internal below
		NormalizeURL(url);


		char *filename = (char *)malloc(10+(strlen(argv[2]))*sizeof(char));
		strcpy(filename, argv[2]);
		strcat(filename, "/.crawler");
		

		//checks if input is valid
		FILE *fp;
		if ( (fp = fopen(filename, "w")) != NULL) {
			if (IsInternalURL(url) == false|| !(maxDepth >= 0 && maxDepth <= 10)) {
				fprintf(stderr, "URL must be an internal URL and maxdepth must be between 1-10. \n");
				return 1;
			}
			fclose(fp);
		} else {
			
			fprintf(stderr, "Directory does not exist!\n");
			return 1;
		}

		//pathname[length] = '\0';
		//printf("printing argv[2] %s \n", argv[2]);

		// FILE *file;
		// check_directory(filename, "/.crawler", file);

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
	    //return false;
  	} 
 	if (toCrawl == NULL) {
  		fprintf(stderr, "bag was not propertly initialized \n");
  		//return false;
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
		pagefetcher(page);
		pagesaver(page, id, pathname);

		//need to increment after saving page so that each page has unique id in directory
		id++;


		if (webpage_getDepth(page) < maxDepth) {
			pagescanner(page, seenUrls, toCrawl);
		} else {
			//printf("deleting page with this url %s \n", webpage_getURL(page));
			
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
		fprintf(stderr, "The webpage was not fetched.");
		return false;
	} else {
		logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
		return true;
	}
	
}


/*  pagesaver method designed to write to file in specified directory;
  	assumption: none of the filenames in directory are integers  */

void
pagesaver(webpage_t *page, int id, char *pathname)
{
	//digits taken from https://stackoverflow.com/questions/3068397/finding-the-length-of-an-integer-in-c
	int digits = floor(log10(abs(id))) + 1;
	char strId[digits];

	sprintf(strId, "%d", id);
	//printf("string id: %s\n", strId);

	char *filename = (char *)malloc((strlen(pathname)+digits+2)*sizeof(char));
	//char filename[digits + strlen(pathname)];

	//creates filename in formation of directory/id
	strcpy(filename, pathname);
	strcat(filename, "/");
	strcat(filename, strId);

	
	//printf("filename: %s \n", filename);


	//printing to file
	FILE * fp;
	if( (fp = fopen(filename, "w")) != NULL ) {
		//add url to beginning of file, followed by depth and HTML
		fprintf(fp, "%s\n", webpage_getURL(page));
		fprintf(fp, "%d\n", webpage_getDepth(page));
		fprintf(fp, "%s\n", webpage_getHTML(page));

		fclose(fp);
	} else {
		//exit if program cannot write to file
		fprintf(stderr, "Error: program could not write to new file in directory");
		exit(1);
	}
	free(filename);		
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
  		//printf("deleting page with this url in scanner %s \n", webpage_getURL(page));
  		//webpage_delete(page);
	}
}

inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %s%9s: %s\n", depth, "", word, url);
}

void deletePage(void *item)
{
  if (item != NULL) {
    webpage_delete(item);
  }
}

/* need empty method to handle the empty string items passed into hashtable's value
   do not need to free the string because it was not allocated with malloc/calloc */
void deleteURL(void *item) {}




