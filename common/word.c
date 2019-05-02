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
	char *temp = (char *)malloc(strlen(word)+1);
	for (char *ptr = word; *ptr != '\0'; ptr++) {
	    temp[ptr - word] = tolower(*ptr);
	}

	// char *temp = (char *)malloc(strlen(word)+1);
	// char temp2[(int)strlen(temp)+1];
	// char *p;

	// int i = 0;
 //    for (p = word; *p != '\0'; p++) {
 //    	temp2[i] = (char)tolower(*p);
 //    	i++;
 //    }

 //    int index=0;
	// while(index <= strlen(temp))
	// {
	//   temp[index] = temp2[index];
	//   index++;
	// }

	//printf ("After conversion temp: %s\n", temp);
	return temp;
}