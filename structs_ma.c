#include <stdio.h>
#include "structs_ma.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

struct Node_ma{
	char name[12];
	int pos;
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
    	tempnod->next=listn;
    	listn=tempnod;
	}
	*array = malloc( sizeof(int *) * items);
	 if (*array == NULL){
        printf("ERROR: out of memory\n");
    }
   // printf("Allocation done!\n");
	for (i=0;i<items;i++){
        (*array)[i] = malloc( sizeof(int) * items);
        /*if ((*array[i]) == NULL){
            printf("ERROR: out of memory\n");
        }*/
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
   /* while(pointer!=NULL){
    	printf("%s,%d\n",pointer->point.name,pointer->point.pos);
    	pointer=pointer->next;
    }*/
    *final_size=items;
	*item=items;
	printf("File Read with success\n");
	fclose(fd);
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
/*	for(i=0;i<L*k;i++){
		printf("%d,%d\n",(*rand_x)[i].x1,(*rand_x)[i].x2);
	}*/
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
	/*for(i=0;i<L*k;i++){
		printf("%d,%d,t1=%f\n",(*rand_x)[i].x1,(*rand_x)[i].x2,(*rand_x)[i].t1);
	}*/
}

void init_hash_ma(List_pointers_ma ****hashtable,int **array,Dist_points *rand_x,int size,int k,int L,int hashsize,List_nodes_ma* listn,int **G_h){
	int i,j;
	*hashtable=malloc(sizeof(List_pointers_ma **)*hashsize);
	/* if (*hashtable == NULL){
        printf("ERROR: out of memory\n");
    }*/
	for(i=0;i<hashsize;i++){
		(*hashtable)[i]=malloc(sizeof(List_pointers_ma*)*L);
	
	
		for(j=0;j<L;j++){
			(*hashtable)[i][j]=NULL;
		}
	}
	printf("%d!\n",hashsize);
	printf("Hashtables allocated\n");
	List_nodes_ma *pointer=listn;
	long int bucket;
	while(pointer!=NULL){
	//	printf("%s,%d\n",pointer->point.name,pointer->point.pos);
		for(i=0;i<L;i++){
			bucket=G_matrix(array,rand_x, pointer->point, G_h,k,i);
		
			List_pointers_ma *temptr;
			temptr=malloc(sizeof(List_pointers_ma));
			temptr->nodeptr=&(pointer->point);
			temptr->next=(*hashtable)[bucket][i];
			(*hashtable)[bucket][i]=temptr;
	//	if(i==1 && bucket==0)	printf("Item %s with position %d in G%d in bucket %d\n",pointer->point.name,pointer->point.pos,i,bucket);
		}
		pointer=pointer->next;
	}
/*	List_pointers_ma *go=(*hashtable)[0][1];
	while(go!=NULL){
		printf("%s, %d->",go->nodeptr->name,go->nodeptr->pos);
		go=go->next;
	}*/
	printf("Data stored in hashtables\n");
	
	
}

int H_matrix(int **array,Dist_points rand_x,Node_ma point){
//	printf("into H\n");
	float d1,d2,d3,d4,sum=0;
			d1=pow(array[point.pos][rand_x.x1],2);
			d2=pow(array[point.pos][rand_x.x2],2);
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
	//	printf("%d",t);
		sum=sum+ t*pow(2,j);

		j--;
	}
	//if(no_G==4 )printf("sum is %li\n",sum);
//	printf("%d",sum);
	return sum;
}

void search_matrix(List_pointers_ma ***hashtable, int **array, Dist_points *rand_x, int **G_h, int k, int L, List_nodes_ma *listn,FILE *input,FILE *output,int size){
	int flag=0,distance,max_distance,i,bucket,position;
	float Radius;
	fscanf(input, "Radius: %f\n",&Radius);
	if (Radius==0) flag=1;
	char bloo[12];
	Node_ma point;
//	int *size;
//	size=malloc(sizeof(int)*size);
	while(!feof(input)){
		fscanf(input, "%s ",bloo);
		printf("%s\n",bloo);
		max_distance=0;
		for(i=0;i<size;i++){
			fscanf(input,"%d",&position);
		//	if(position==0)
		}
		for(i=0;i<L;i++){
			
		}
		
	}
	
}

int matrix_distance(int pos1, int pos2, int **array){
	return array[pos1][pos2];
}
