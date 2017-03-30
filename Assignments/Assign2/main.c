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
	char* filename;
	int count;
	struct lnode* link;
} linkNode;


typedef struct hnode{
	struct hnode* front;
	char* word;
	struct hnode* next;
	linkNode* link;
} hashNode;


hashNode *hashTable = NULL;

hashNode* newHashNode(hashNode*, char*, hashNode*, linkNode*);
linkNode* newLinkNode(hashNode*, char*, int, linkNode*);
hashNode* searchHash(hashNode*, char*);
char * getnewpath(char *, char *);
static void recursiveSearch(char*);


hashNode* searchHash(hashNode* front, char* keyword){
	hashNode* tmp = front;
	while(tmp->next!=NULL){
		if(tmp->word == keyword){
			return tmp;
		}else{
			tmp=tmp->next;
		}
	}
	return NULL;
}

linkNode* newLinkNode(char* filename, int count, linkNode* link){
	linkNode* newnode;
	newnode = (linkNode*)malloc(sizeof(linkNode));
	newnode->filename = filename;
	newnode->count = count;
	newnode->link = link;
	return newnode;
}


hashNode* newHashNode(hashNode* front, char* word, hashNode* next, linkNode* link){
	hashNode *newnode;
	newnode = (hashNode*) malloc(sizeof(hashNode));
	newnode->word = word;
	newnode->next = next;
	newnode->link = link;
	if(front==NULL){
		front = newnode;
		return newnode;
	}else{
		while(front->next!=NULL){
			front=front->next;
		}
		front->next=newnode;
	}	
	return newnode;
}


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
				if(searchHash(hashTable,tmp->token)==NULL){//couldnt find word
					hashNode* newHash = newHashNode(hashTable,tmp->token, NULL, NULL); 
					linkNode* first = newLinkNode(d_name, 1, NULL);
					newHash->link=first;
				}else{
					hashNode* word = searchHash(hashTable,tmp->token); 
					linkNode* temp = word->link;
					while(temp->link!=NULL){
						
						if(temp->filename == d_name){
							temp->count += 1; //if the filename is found amongst the word hash, increment
							break;
						}else{
							temp = temp->link;
						}
					}
					if(temp->link==NULL){
						linkNode* newFile = newLinkNode(word,d_name,1, NULL); //inserts a newLink if the file is new (or not found in the linkNode list)
						temp->link = newFile;
					}
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
