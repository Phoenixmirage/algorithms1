#include "structs.h"
#include "functions.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct euc_vec{
	float *vector;
	float t;
};

struct Node{
	char name[12];
	int visited;
	float* array;
};

struct List_nodes{
	Node point;
	List_nodes *next;
};

struct List_pointers{
	long int id;
	Node *nodeptr;
	List_pointers *next;
};

int F_Euclidean(unsigned long int ID,int hash_size){
	return ID%hash_size;
}
unsigned long int ID_euclidean(int **G_h, int no_G, Node p, int size, long int *random_r, int k,euc_vec *vectors,int W){
	unsigned long int M=pow(2,32)-5;
	int i,j;
	unsigned long int sum=0;
	float t;
	for(i=0;i<k;i++){
		t= H_euclidean(vectors[G_h[no_G][i]],p,size,W);
		t=t*random_r[i];
		sum= sum+ t;
	}
	if(sum<0) sum=sum*(-1);
		return sum%M;
}

long int H_euclidean(euc_vec vector_t, Node p,int size,int W){
	int i;
	long int j;
	float sum=0;
	for(i=0; i<size; i++){
		sum=sum+ (vector_t.vector[i]*p.array[i]);
	}
	sum=(sum+vector_t.t)/W;
	j=sum;
	return j;
}

float euclidean_distance(float *point, float *item, int size){
	int i;
	float sum=0,square;
	for(i=0; i<size; i++){
		square= point[i]-item[i];
		square=pow(square,2);
		sum=sum + square;
	}
	sum=sqrt(sum);
	return sum;
}

List_nodes* Euclidean_input(FILE *fd,int* final_size, int * item){
		int items=0;
		List_nodes *listn=NULL;
		int size=0;
		int tempsize=2;
		char bla[12];
		float * array;
		array=malloc(tempsize*sizeof(float));
		fscanf(fd, "%s",bla);
		items++;
		char c;
		c='t';
		while(c!='\n')
		{
				fscanf(fd, "%f%c", &(array[size]), &c);
				size++;
				if (size==tempsize-1){
						tempsize*=2;
						array=realloc(array, tempsize*sizeof(float));
				}
		}	
		array=realloc(array, size*sizeof(float));
		List_nodes *tempnod;
		tempnod=malloc(sizeof(List_nodes));
		strcpy(tempnod->point.name,bla);
		memset(bla, 0, sizeof(bla));
		tempnod->point.array=array;
		tempnod->point.visited=0;
		tempnod->next=listn;
		listn=tempnod;
		int i;
		while(!feof(fd)){
			fscanf(fd, "%s", bla);
			items++;
			if (!strcmp(bla,"")){
				items--;
					break;
			}
			tempnod=malloc(sizeof(List_nodes));
			strcpy(tempnod->point.name,bla);
			memset(bla, 0, sizeof(bla));
			tempnod->point.array=malloc(size*sizeof(float));
			tempnod->point.visited=0;
			for(i=0;i<size;i++)
			{
					fscanf(fd, "%f", &(tempnod->point.array[i]));
			}
			tempnod->next=listn;
			listn=tempnod;
		}
		*final_size=size;
		*item=items;
		printf("File Read with success\n");
		return listn;
	}
	
void init_randvec(euc_vec **randvec,int L, int k,int W,int size){
	int i,j;
	(*randvec)=malloc(L*k*(sizeof(euc_vec)));
    for(i=0;i<L*k;i++){
        (*randvec)[i].vector=malloc(size*sizeof(float));
    }
    for(i=0;i<L*k;i++){
        (*randvec)[i].t=((float)rand()/(float)(RAND_MAX)*W);
        for(j=0;j<size;j++){
        	(*randvec)[i].vector[j]=marsaglia();
        }	
    }
}

void init_hash(List_pointers ****hashtable,euc_vec *randvec,int size,int k,int L,int hashsize,List_nodes *listn,int **G_h,int W,long int *random_r){
		int i,j;
	*hashtable=malloc(sizeof(List_pointers **)*hashsize);
	for(i=0;i<hashsize;i++){
		(*hashtable)[i]=malloc(sizeof(List_pointers*)*L);
			for(j=0;j<L;j++){
			(*hashtable)[i][j]=NULL;
		}
	}
	printf("Hashtables allocated\n");
	List_nodes *pointer=listn;
	int bucket;
	long int g;
	while(pointer!=NULL){
		for(i=0;i<L;i++){
			g=ID_euclidean(G_h,i, pointer->point, size,random_r,k,randvec,W);
			bucket= F_Euclidean(g,hashsize);
			List_pointers *temptr;
			temptr=malloc(sizeof(List_pointers));
			temptr->nodeptr=&(pointer->point);
			temptr->id=g;
			temptr->next=(*hashtable)[bucket][i];
			(*hashtable)[bucket][i]=temptr;
		}
		pointer=pointer->next;
	}
	printf("Data stored in hashtables\n");
}

void search_euclidean(List_pointers ***hashtables,FILE *input,List_nodes *listn,int k,int L,int size,int W,euc_vec *randvec,long int *random_r,int hashsize,int **G_h,FILE *output){
	int i,j,bucket,id;
    	char radius[20];
	int flag=0;
    	fscanf(input, "Radius: %s\n",radius);
    	double time_spent,time_spent1;
   	float Radius= atof(radius);
    float distance, max_distance=1000;
    List_pointers *neighbor;
    clock_t begin, begin1, end, end1;
    if (Radius==0) flag=1;
    char bloo[12];
    while(!feof(input)){
    	memset(bloo, 0, sizeof(bloo));
    	fscanf(input, "%s",bloo);
    	if (!strcmp(bloo,"")){
            break;
        }
        Node point;
        point.array=malloc(size*sizeof(float));
        for(i=0;i<size;i++)
        {
        	fscanf(input, "%f", &point.array[i]);
        }
        List_nodes *pointer=listn;
        while(pointer!=NULL){
       			
        		pointer->point.visited=0;
        		pointer=pointer->next;
        		
		}
        max_distance=1000;
        begin=clock();
        for(i=0;i<L;i++){
        	id=ID_euclidean(G_h,i, point, size,random_r,k,randvec,W);
			bucket= F_Euclidean(id,hashsize);
			List_pointers *go=hashtables[bucket][i];
			while(go!=NULL){
				if(id==go->id){
					if(go->nodeptr->visited==0){
						distance=euclidean_distance(point.array,go->nodeptr->array,size);
						if(distance<max_distance && distance!=0){
							max_distance=distance;
							neighbor=go;
						}
						go->nodeptr->visited=1;
					}
				}
				go=go->next;
			}
		}
		end=clock();
    	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    	pointer=listn;
		float max_distance1=1000;
		begin1=clock();
       	while(pointer!=NULL){
       		distance=euclidean_distance(point.array,pointer->point.array,size);
        	if(distance<=max_distance1 && distance!=0){
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
				id=ID_euclidean(G_h,i, point, size,random_r,k,randvec,W);
				bucket= F_Euclidean(id,hashsize);
				List_pointers *go=hashtables[bucket][i];
				while(go!=NULL){
					if(id==go->id){
						if(go->nodeptr->visited==0){
							distance=euclidean_distance(point.array,go->nodeptr->array,size);
							if(distance<=Radius){
								fflush(output);
								fprintf(output,"%s\n",go->nodeptr->name);
							}
							go->nodeptr->visited=1;
						}
					}
					go=go->next;
				}
			}
		}
		fflush(output);
		fprintf(output,"Nearest neighbor: %s\nDistanceLSH: %f\n",neighbor->nodeptr->name,max_distance);
		fprintf(output,"DistanceTrue: %f\ntLSH: %f\ntTrue:%f\n",max_distance1,time_spent,time_spent1);
		free(point.array);
	}
	printf("File written successfully\n");
}

void free_hash(List_pointers  ****hashtable, int hashsize,int L){
	int i,j;
	List_pointers *temp;
	for(i=0;i<hashsize;i++){
		for(j=0;j<L;j++){
			temp=(*hashtable)[i][j];
			while(temp!=NULL){
				List_pointers *temptemp;
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

void free_list_nodes(List_nodes **listn, int size){
	List_nodes *templist;
	int i;
	while((*listn)!=NULL){
		templist=(*listn);
		(*listn)=(*listn)->next;
		free(templist->point.array);
		free(templist);
	}
}

void free_randvec(euc_vec **randvec, int L, int k){
	int i;
	for(i=0;i<L*k;i++){
		free((*randvec)[i].vector);
	}
	free(*randvec);
	(*randvec)=NULL;
}
