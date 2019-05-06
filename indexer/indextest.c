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

		//also checks if oldIndexFilename exists
		if ((lines = get_num_lines(oldIndexFilename)) != -1) {
			//determines the size of the hashtable based on words in file
			if (lines > 10) {
				sizeOfTable = (int) (lines/10);
			} else {
				sizeOfTable = lines;
			}

		} else {
			fprintf(stderr, "The file %s did not exist\n", oldIndexFilename);
			free(oldIndexFilename);
			free(newIndexFilename);
			return 1;
		}

		FILE *fp;
		//if the file exists, check whether it is writable or not --> creates writable file if it doesn't exist
		if ((fp = fopen(newIndexFilename, "w")) == NULL) {
			fprintf(stderr, "File %s could not be written to!\n", newIndexFilename);
			free(oldIndexFilename);
			free(newIndexFilename);
			return 1;
		}
		fclose(fp);


		//create new index to pass into index_load and index_save based on words in file
		index_t *index = index_new(sizeOfTable);

		index_load(oldIndexFilename, index);
		index_save(newIndexFilename, index);

		index_delete(index);
		free(oldIndexFilename);
		free(newIndexFilename);

	} else {
		fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename");
	}
}