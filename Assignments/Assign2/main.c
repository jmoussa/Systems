#include "count.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

char * getnewpath(char *, char *);
static void recursiveSearch(char*);

static void recursiveSearch(char* dirname){
	DIR * dir;
	dir = opendir(dirname);
	Node * head;
	char* newpath;
	if(!dir){
		printf("Error Cannot Open Directory");
		exit(EXIT_FAILURE);
	}
	struct dirent* entry;
	const char * d_name; //name of the file

	entry = readdir(dir);

	while(entry!=NULL){

		d_name = entry->d_name;
		
		if((entry->d_type & DT_REG)){
			head = countFromSingleFile(d_name);
			Node* tmp=head;
			while (tmp!=NULL) {
				printf("%s\t\t%d\n", tmp->token,tmp->count);
				tmp=tmp->next;
			}
		}
		
		if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
			newpath = getnewpath(dirname, entry->d_name);
			recursiveSearch(newpath);
			free(newpath);
			newpath = NULL;	
		}
	}
	if(closedir(dir)){
		fprintf(stderr, "Couldn't close '%s': %s\n", dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}

}

char * getnewpath(char * dirname, char * newdir){
	char * newpath = (char *) calloc(strlen(dirname) + strlen(newdir) + 2, 1);
	newpath = strcpy(newpath, dirname);
	newpath = strcat(newpath, "/");
	newpath = strcat(newpath, newdir);
	return newpath;
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


	//BUILD INDEX
	



	//TODO Save index to a file (write and save to a file)

	return 0;
}
