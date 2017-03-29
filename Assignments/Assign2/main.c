#include "count.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

static void recursiveSearch(char* dirname){
	DIR * dir;
	dir = opendir(dirname);
	Node * head;
	if(!dir){
		printf("Error Cannot Open Directory");
		exit(EXIT_FAILURE);
	}

	while(1){
		struct dirent* entry;
		const char * d_name; //name of the file

		entry = readdir(dir);
		if(!entry){
			//no more entries in this dir
			break;
		}

		d_name = entry->d_name;
		
		if((entry->d_type & DT_REG)){
			head = countFromSingleFile(d_name);
			Node* tmp=head;
			while (tmp!=NULL) {
				printf("%s\t\t%d\n", tmp->token,tmp->count);
				tmp=tmp->next;
			}
		}
		
		if(entry->d_type & DT_DIR){
			if(strcmp(d_name, "..")!=0 && strcmp(d_name, ".")!=0){
				int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,"%s/%s", dirname, d_name);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
				recursiveSearch(path);
			}
		}
	}
	if(closedir(dir)){
		fprintf(stderr, "Couldn't close '%s': %s\n", dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}

}


int main(int argc, char const *argv[])
{
	// ****************************************************************
	// All tokens in the file and their counts will be
	// stored in the LinkedList 'head'
	// ****************************************************************
	//Node* head = countFromSingleFile("./testcases/1.txt");


	// ****************************************************************
	// Print TOKENs and COUNTs
	// ****************************************************************
	/*
	Node* tmp=head;
	while (tmp!=NULL) {
		printf("%s\t\t%d\n", tmp->token,tmp->count);
		tmp=tmp->next;
	}
   	*/

	return 0;
}
