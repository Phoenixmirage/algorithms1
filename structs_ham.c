#include "structs_ham.h"
#include "functions.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node_Ham{
	char name[12];
	unsigned long long int binarystr;
	int visited;
};

struct List_nodes_Ham{
	Node_Ham point;
	List_nodes_Ham *next;
};

struct List_pointers_Ham{
	Node_Ham *nodeptr;
	List_pointers_Ham *next;
};

long int G_hamming(int **G_h, int no_G, int number, int size, int k){
	int i,j,t;
	j=k-1;
	long int sum=0;
	for(i=0;i<k;i++){
		t= H_hamming(number,size,G_h[no_G][i]);
		sum=sum+ t*pow(2,j);
		j--;
	}
	return sum;
}

int H_hamming(unsigned long long int number, int size, int pos){
	int i;
	unsigned long long int k=1,o;
	k=k<<(size-pos);
	o=number&k;
	if(o==0) return 0;
	else return 1;
	
}

int hamming_distance(unsigned long long int number1, unsigned long long int number2, int size){
	int distance=0,i;
	unsigned long long int a_xor, final=1;
	a_xor=number1^number2;
	for(i=0; i<size; i++){
		if((a_xor&final)==0) distance++;
		final=final<<1;
	}
	return size-distance;
}

List_nodes_Ham* Hamming_input(FILE *fd,int* final_size, int * item){
	char bin_string[65];
    char bla[12];
    int items=0;
    int size=0;
    int i,j;
    List_nodes_Ham *listn=NULL;
	fscanf(fd, "%s",bla);
   	fscanf(fd,"%s\n",bin_string);
    size=strlen(bin_string);
    items++;
    List_nodes_Ham *tempnode;
    tempnode=malloc(sizeof(List_nodes_Ham));
    strcpy(tempnode->point.name,bla);
    memset(bla, 0, sizeof(bla));
    tempnode->point.binarystr=string_to_int(bin_string,size);
    tempnode->point.visited=0;
    tempnode->next=listn;
   	listn=tempnode;
    while(!feof(fd)){
    	fscanf(fd, "%s", bla);
        items++;
        if (!strcmp(bla,"")){
        	items--;
            break;
        }
        fscanf(fd,"%s\n",bin_string);
       	tempnode=malloc(sizeof(List_nodes_Ham));
    	tempnode->point.binarystr=string_to_int(bin_string,size);
        tempnode->point.visited=0;
        strcpy(tempnode->point.name,bla);
        memset(bla, 0, sizeof(bla));
		tempnode->next=listn;
        listn=tempnode;
    }
    *final_size=size;
	*item=items;
	printf("File Read with success\n");
	return listn;
}

void init_hash_Ham(List_pointers_Ham ****hashtable,int size,int k,int L,int hashsize,List_nodes_Ham *listn,int **G_h){
	int i,j;
	*hashtable=malloc(sizeof(List_pointers_Ham **)*hashsize);
	for(i=0;i<hashsize;i++){
		(*hashtable)[i]=malloc(sizeof(List_pointers_Ham*)*L);
			for(j=0;j<L;j++){
			(*hashtable)[i][j]=NULL;
		}
	}
	printf("Hashtables allocated\n");
	List_nodes_Ham *pointer=listn;
	long int bucket;
	long int g;
	while(pointer!=NULL){
		for(i=0;i<L;i++){
			bucket=G_hamming(G_h,i, pointer->point.binarystr, size,k);
			List_pointers_Ham *temptr;
			temptr=malloc(sizeof(List_pointers_Ham));
			temptr->nodeptr=&(pointer->point);
			temptr->next=(*hashtable)[bucket][i];
			(*hashtable)[bucket][i]=temptr;
		}
		pointer=pointer->next;
	}
	printf("Data stored in hashtables\n");
}

void search_Ham(List_pointers_Ham ***hashtables,FILE *input,List_nodes_Ham *listn,int k,int L,int size,int hashsize,int **G_h,FILE *output){
	int i,j,bucket;
    char radius[20];
	int flag=0;
    fscanf(input, "Radius: %s\n",radius);
    double time_spent,time_spent1;
    float Radius= atof(radius);
    int distance, max_distance=64;
    List_pointers_Ham *neighbor;
    clock_t begin, begin1, end, end1;
    if (Radius==0) flag=1;
    char bloo[12];
    unsigned long long int number;
    while(!feof(input)){
    	memset(bloo, 0, sizeof(bloo));
    	fscanf(input, "%s",bloo);
    	if (!strcmp(bloo,"")){
            break;
        }
        Node_Ham point;
        char bin_string[65];
        fscanf(input,"%s\n",bin_string);
        number= string_to_int(bin_string,size);
        max_distance=64;
        begin=clock();
        List_nodes_Ham *pointer=listn;
        while(pointer!=NULL){
       			
        		pointer->point.visited=0;
        		pointer=pointer->next;
        		
		}
        for(i=0;i<L;i++){
        	bucket=G_hamming(G_h,i, number, size,k);
			List_pointers_Ham *go=hashtables[bucket][i];
			while(go!=NULL){
					if(go->nodeptr->visited==0){
						distance=hamming_distance(go->nodeptr->binarystr,number,size);
						if(distance<max_distance && distance!=0){
							
							max_distance=distance;
							neighbor=go;
						}
						go->nodeptr->visited=1;
					}
				go=go->next;
			}
		}
		end=clock();
    	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    	pointer=listn;
		int max_distance1=64;
		begin1=clock();
       	while(pointer!=NULL){
       			distance=hamming_distance(pointer->point.binarystr,number,size);
        		if(distance<max_distance1 && distance!=0){
        			max_distance1=distance;
        		}	
        		pointer->point.visited=0;
        		pointer=pointer->next;
        		
		}
		end1=clock();
		time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
		fflush(output);
        fprintf(output,"\nQuery: %s\n",bloo);
        if (flag==0){
        	fflush(output);
        	fprintf(output,"R-nearest neighbours:\n");
        	for(i=0;i<L;i++){
        		bucket=G_hamming(G_h,i, number, size,k);
				List_pointers_Ham *go=hashtables[bucket][i];
				while(go!=NULL){
					if(go->nodeptr->visited==0){
						distance=hamming_distance(go->nodeptr->binarystr,number,size);
						if(distance<=Radius){
							fflush(output);
							fprintf(output,"%s\n",go->nodeptr->name);
						}
						go->nodeptr->visited=1;
					}
					go=go->next;
				}
			}
		}
		fflush(output);
		fprintf(output,"Nearest neighbor: %s\nDistanceLSH: %d\n",neighbor->nodeptr->name,max_distance);
		fprintf(output,"DistanceTrue: %d\ntLSH: %f\ntTrue:%f\n",max_distance1,time_spent,time_spent1);
	}
	printf("File written successfully\n");
}

void free_hash_ham(List_pointers_Ham  ****hashtable, int hashsize,int L){
	int i,j;
	List_pointers_Ham *temp;
	for(i=0;i<hashsize;i++){
		for(j=0;j<L;j++){
			temp=(*hashtable)[i][j];
			while(temp!=NULL){
				List_pointers_Ham *temptemp;
				temptemp=temp;
				temp=temp->next;
				free(temptemp);
			}
		}
		free((*hashtable)[i]);
	}
	free(*hashtable);
	(*hashtable)=NULL;
}

void free_list_nodes_ham(List_nodes_Ham **listn, int size){
	List_nodes_Ham *templist;
	int i;
	while((*listn)!=NULL){
		templist=(*listn);
		(*listn)=(*listn)->next;
		free(templist);
	}
}

