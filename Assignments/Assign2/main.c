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
#include <fcntl.h>

typedef struct lnode{
	char filename[100];
	int count;
	struct lnode* link;
} linkNode;


typedef struct hnode{
	char word[100];
	struct hnode* next;
	linkNode* link;
} hashNode;


hashNode *hashTable = NULL;

char * getnewpath(char *, char *);
static void recursiveSearch(char*);

void recursiveSearch(char* dirname){
	DIR * dir;
	dir = opendir(dirname);
	Node * head;
	char* newpath;
	if(!dir){
		printf("Error Cannot Open Directory");
		exit(EXIT_FAILURE);
	}
	struct dirent* entry;
	char * d_name; //name of the file

	entry = readdir(dir);

	while(entry!=NULL){

		d_name = entry->d_name;
		
		if((entry->d_type & DT_REG)){
			head = countFromSingleFile(d_name);
			Node* tmp=head;
			while (tmp!=NULL) {
				if(searchHash(hashTable,tmp->token)==-1){//couldnt find word
					hashNode* top = newHashNode(hashTable,tmp->token, NULL, NULL);
					linkNode* first = newLinkNode(top,d_name, 1, NULL);
					top->link=first;
				}else{
					hashNode* word = searchHash(hashTable,tmp->token);
						
				}

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
	Node* head = countFromSingleFile("./testcases/1.txt");
	

	// ****************************************************************
	// Print TOKENs and COUNTs
	// ****************************************************************
	Node* tmp=head;
	while (tmp!=NULL) {
		printf("<Word Text= %s > \n\t <file name = %s > %d\n", tmp->token,argv[2],tmp->count);
		tmp=tmp->next;
	}


	//BUILD INDEX
	/*
	if(open(argv[2],O_RDONLY)==-1){
		//directory found
		recursiveSearch(argv[2]);
	}else{
		Node* head = countFromSingleFile(argv[2]);
		Node* tmp=head;
		while (tmp!=NULL) {
			printf("%s\t\t%d\n", tmp->token,tmp->count);
			tmp=tmp->next;
		}

	}
	*/


	//TODO Save index to a file (write and save to a file)

	return 0;
}
