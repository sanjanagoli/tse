/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - word.c
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include <ctype.h>


char * 
NormalizeWord(char *word)
{
	char *temp = (char *)malloc((strlen(word)+1)*sizeof(char));
	strcpy(temp, word);
	for (char *ptr = word; *ptr != '\0'; ptr++) {
	    temp[ptr - word] = tolower(*ptr);
	}


	//printf ("After conversion temp: %s\n", temp);
	return temp;
}