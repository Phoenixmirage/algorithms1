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
	//	printf("hi is %d and first dimension of vector is %f\n",G_h[no_G][i], vectors[G_h[no_G][i]].vector[0]);
	//	printf("random is %d\n",random_r[i]);
		t= H_hamming(number,size,G_h[no_G][i]);
	//	printf("hi*p is %f\n",t);
		sum=sum+ t*pow(2,j);
	//	printf("hi*p*r is %f\n",t);
		j--;
	}
	//if(no_G==4 )printf("sum is %li\n",sum);
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
        items++;//	printf("%s",bla);
        if (!strcmp(bla,"")){
        	items--;
            break;
        }
        fscanf(fd,"%s\n",bin_string);
       	//	printf("%s\n",bin_string);
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
	fclose(fd);
	return listn;
    fclose(fd);
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
//	printf("%d!\n",hashsize);
	printf("Hashtables allocated\n");
	List_nodes_Ham *pointer=listn;
	long int bucket;
	long int g;
	while(pointer!=NULL){
	//	printf("%s,%d\n",pointer->point.name,pointer->point.pos);
		for(i=0;i<L;i++){
			bucket=G_hamming(G_h,i, pointer->point.binarystr, size,k);
		//	if(i==3 && bucket==2) printf("Item %s G%d, to bucket %d\n",pointer->point.name,i,bucket);
			List_pointers_Ham *temptr;
			temptr=malloc(sizeof(List_pointers_Ham));
			temptr->nodeptr=&(pointer->point);
			temptr->next=(*hashtable)[bucket][i];
			(*hashtable)[bucket][i]=temptr;
			
	//	if(i==1 && bucket==0)	printf("Item %s with position %d in G%d in bucket %d\n",pointer->point.name,pointer->point.pos,i,bucket);
		}
		pointer=pointer->next;
	}
//	List_pointers_Ham *go=(*hashtable)[2][3];
/*	while(go!=NULL){
		printf("%s, %lli->",go->nodeptr->name,go->nodeptr->binarystr);
		go=go->next;
	}*/
	printf("Data stored in hashtables\n");
}

void search_Ham(List_pointers_Ham ***hashtables,FILE *input,List_nodes_Ham *listn,int k,int L,int size,int hashsize,int **G_h,FILE *output){
	int i,j,bucket;
    char radius[20];
	int flag=0;
    fscanf(input, "Radius: %s\n",radius);
   // printf("%s\n",radius);
    double time_spent,time_spent1;
    float Radius= atof(radius);
    int distance, max_distance=64;
    List_pointers_Ham *neighbor;
    clock_t begin, begin1, end, end1;
 //   printf("%f\n",Radius);
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
						//printf("%s,%d\n",go->nodeptr->name,distance);
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
        		//printf("G%d,bucket%d\n",i,bucket);
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
}

