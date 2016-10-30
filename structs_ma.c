#include <stdio.h>
#include "structs_ma.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

struct Node_ma{
	char name[12];
	int *array;
	int pos;
	int visited;
};

struct List_nodes_ma{
	Node_ma point;
	List_nodes_ma *next;
};

struct List_pointers_ma{
	Node_ma *nodeptr;
	List_pointers_ma *next;
};

struct Dist_points{
	int x1;
	int x2;
	float t1;
};

List_nodes_ma* matrix_input(FILE *fd,int* final_size, int * item,int ***array){
	int items=0,i,j;
	List_nodes_ma *listn=NULL;
    int size=0;
    char bla[12];
    char c;
    c='t';
    fscanf(fd, "%s ",bla);
	while(c!='\n')
    {
    	fscanf(fd, "%[^,\n]%c", bla,&c);
        items++;
	   	List_nodes_ma *tempnod;
    	tempnod=malloc(sizeof(List_nodes_ma));
    	strcpy(tempnod->point.name,bla);
    	memset(bla, 0, sizeof(bla));
    	tempnod->point.pos=items-1;
	tempnod->point.visited=0;
    	tempnod->next=listn;
    	listn=tempnod;
	}
	*array = malloc( sizeof(int *) * items);
	 if (*array == NULL){
        printf("ERROR: out of memory\n");
    }
	for (i=0;i<items;i++){
        (*array)[i] = malloc( sizeof(int) * items);
    }
	i=0;
	char d[4];
    for(i=0;i<items;i++){
    	for(j=0;j<items;j++)
        {
        	fscanf(fd, "%s", d);
        	if (!strcmp(d,"")) break;
        	(*array)[i][j]=atoi(d);
        }
    }
    List_nodes_ma *pointer=listn;
    while(pointer!=NULL){
    	pointer->point.array=malloc(items*sizeof(int));
		for(i=0;i<items;i++){	
			pointer->point.array[i]=(*array)[pointer->point.pos][i];
		}
    	pointer=pointer->next;
    }
    *final_size=items;
	*item=items;
	printf("File Read with success\n");
	return listn;
}

void rand_x1_x2(int **array,Dist_points **rand_x, int L, int k, int size){
	(*rand_x)=malloc(sizeof(Dist_points)*L*k);
	int i,j;
	for(i=0;i<L*k;i++){
		(*rand_x)[i].x1=rand()%size;
		do{
			(*rand_x)[i].x2=rand()%size;
		}while( (*rand_x)[i].x1==(*rand_x)[i].x2);
	}
	int d1,d2,d3,d4;
	float sum;
	for(i=0;i<L*k;i++){
		sum=0;
		for(j=0;j<size;j++){
			d1=pow(array[j][(*rand_x)[i].x1],2);
			d2=pow(array[j][(*rand_x)[i].x2],2);
			d3=array[(*rand_x)[i].x1][(*rand_x)[i].x2];
			d4=((d1+d2-pow(d3,2))/(2*d3));
		    sum=sum + d4;
		}
		(*rand_x)[i].t1=sum/size;
	}
}

void init_hash_ma(List_pointers_ma ****hashtable,int **array,Dist_points *rand_x,int size,int k,int L,int hashsize,List_nodes_ma* listn,int **G_h){
	int i,j;
	*hashtable=malloc(sizeof(List_pointers_ma **)*hashsize);
	for(i=0;i<hashsize;i++){
		(*hashtable)[i]=malloc(sizeof(List_pointers_ma*)*L);
	
	
		for(j=0;j<L;j++){
			(*hashtable)[i][j]=NULL;
		}
	}
	printf("Hashtables allocated\n");
	List_nodes_ma *pointer=listn;
	long int bucket;
	while(pointer!=NULL){
		for(i=0;i<L;i++){
			bucket=G_matrix(array,rand_x, pointer->point, G_h,k,i);
			List_pointers_ma *temptr;
			temptr=malloc(sizeof(List_pointers_ma));
			temptr->nodeptr=&(pointer->point);
			temptr->next=(*hashtable)[bucket][i];
			(*hashtable)[bucket][i]=temptr;
		}
		pointer=pointer->next;
	}
	printf("Data stored in hashtables\n");	
}

int H_matrix(int **array,Dist_points rand_x,Node_ma point){
	float d1,d2,d3,d4,sum=0;
	d1=pow(point.array[rand_x.x1],2);
	d2=pow(point.array[rand_x.x2],2);
	d3=array[rand_x.x1][rand_x.x2];
	sum=((d1+d2-pow(d3,2))/(2*d3));
	if (sum>=rand_x.t1) return 1;
	else return 0;
}

long int G_matrix(int **array,Dist_points *rand_x, Node_ma point, int **G_h,int k,int no_G){
	int i,j,t;
	j=k-1;
	long int sum=0;
	for(i=0;i<k;i++){
		t= H_matrix(array,rand_x[G_h[no_G][i]],point);
		sum=sum+ t*pow(2,j);
		j--;
	}
	return sum;
}

void search_matrix(List_pointers_ma ***hashtable, int **array, Dist_points *rand_x, int **G_h, int k, int L, List_nodes_ma *listn,FILE *input,FILE *output,int size){
	int flag=0,distance,max_distance,i,position;
	float Radius;
	fscanf(input, "Radius: %f\n",&Radius);
	if (Radius==0) flag=1;
	double time_spent,time_spent1;
	clock_t begin, begin1, end, end1;
	char bloo[12];
	long int bucket;
	List_pointers_ma *neighbor;
	Node_ma point;
	point.array=malloc(sizeof(int)*size);
	while(!feof(input)){
		memset(bloo, 0, sizeof(bloo));
		fscanf(input, "%s ",bloo);
		if (!strcmp(bloo,"")){
           		 break;	
		}
		max_distance=0;
		for(i=0;i<size;i++){
			fscanf(input,"%d",&point.array[i]);
		}
		max_distance=1000;
		List_nodes_ma *pointer=listn;
        	while(pointer!=NULL){
       			pointer->point.visited=0;
        		pointer=pointer->next; 		
		}
		begin=clock();
		for(i=0;i<L;i++){
			bucket=G_matrix(array,rand_x, point, G_h,k,i);
			List_pointers_ma *go=hashtable[bucket][i];
			while(go!=NULL){
				if(go->nodeptr->visited==0){
					distance=point.array[go->nodeptr->pos];
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
		int max_distance1=1000;
		begin1=clock();
		pointer=listn;
       	while(pointer!=NULL){
       		distance=point.array[pointer->point.pos];
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
				bucket=G_matrix(array,rand_x, point, G_h,k,i);
				List_pointers_ma *go=hashtable[bucket][i];
				while(go!=NULL){
					if(go->nodeptr->visited==0){
						distance=point.array[go->nodeptr->pos];
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
	free(point.array);
}

void free_hash_ma(List_pointers_ma  ****hashtable, int hashsize,int L){
	int i,j;
	List_pointers_ma *temp;
	for(i=0;i<hashsize;i++){
		for(j=0;j<L;j++){
			temp=(*hashtable)[i][j];
			while(temp!=NULL){
				List_pointers_ma *temptemp;
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

void free_list_nodes_ma(List_nodes_ma **listn, int size){
	List_nodes_ma *templist;
	int i;
	while((*listn)!=NULL){
		templist=(*listn);
		(*listn)=(*listn)->next;
		free(templist->point.array);
		free(templist);
	}
}

void free_matrix_array(int ***array,int size){
	int i;
	for(i=0;i<size;i++)
		free((*array)[i]);
	free(*array);
}