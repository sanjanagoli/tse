/*
* Sanjana Goli, April 2019
* Lab 4 (Tiny Search Engine) - pagedir.h
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include <string.h>
#include <math.h>

/* check_directory: takes in string filename, returns
					whether directory exists */
bool check_directory(char *filename);


void savepage(webpage_t *page, int id, char *pathname);

void test_method();