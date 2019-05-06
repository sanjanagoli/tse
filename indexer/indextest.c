/*
* Sanjana Goli, April 2019
* Lab 5 (Tiny Search Engine) - indextest.c
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "webpage.h"
#include "counters.h"

int
main(int argc, char *argv[])
{
	if(argc == 3) {

		char *oldIndexFilename = (char *)malloc((strlen(argv[1])+1)*sizeof(char));
		strcpy(oldIndexFilename, argv[1]);
		char *newIndexFilename = (char *)malloc((strlen(argv[2])+1)*sizeof(char));
		strcpy(newIndexFilename, argv[2]);

		int lines;
		int sizeOfTable;
		if ((lines = get_num_lines(oldIndexFilename)) != -1) {
			sizeOfTable = (int) (lines/10);

		} else {
			fprintf(stderr, "There were no lines in the %s file", oldIndexFilename);
			return 1;
		}
		
		index_t *index2 = index_new(sizeOfTable);

		index_load(oldIndexFilename, index2);
		index_save(newIndexFilename, index2);

		index_delete(index2);
		free(oldIndexFilename);
		free(newIndexFilename);

	} else {
		fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename");
	}
}