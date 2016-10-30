#include "structs_cos.h"
#include "functions.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct cos_vec{
	float *vector;
};

struct Node_cos{
	char name[12];
	int visited;
	float* array;
};

struct List_nodes_cos{
	Node_cos point;
	List_nodes_cos *next;
};

struct List_pointers_cos{
	Node_cos *nodeptr;
	List_pointers_cos *next;
};

long int G_cosine(int **G_h, int no_G, Node_cos p, int size, int k,cos_vec *vectors){
	int i,j;
	j=k-1;
	long int sum=0;
	float t;
	for(i=0;i<k;i++){
	//	printf("hi is %d and first dimension of vector is %f\n",G_h[no_G][i], vectors[G_h[no_G][i]].vector[0]);
	//	printf("random is %d\n",random_r[i]);
		t= H_cosine(vectors[G_h[no_G][i]],p,size);
	//	printf("hi*p is %f\n",t);
		sum=sum+ t*pow(2,j);
	//	printf("hi*p*r is %f\n",t);
		j--;
	}
	//if(no_G==4 )printf("sum is %li\n",sum);
	return sum;
}
 int H_cosine(cos_vec vector_t, Node_cos p,int size){
	int i;
	float sum=0;
	for(i=0; i<size; i++){
	//	printf("%f * %f\n",vector_t.vector[i],p.array[i]);
		sum=sum+ (vector_t.vector[i]*p.array[i]);
	}
	
	if (sum>=0){
	//	printf("1");
		return 1;
	}
	else{
	//	printf("0");
		return 0;
	}
}

float cosine_distance(float *point, float *item, int size){
	int i,j=0;
	float sum=0,square,meterA=0,meterB=0;
	for(i=0; i<size; i++){
		if(point[i]==item[i]) j++;
		meterA= meterA + pow(point[i],2);
		meterB= meterB + pow(item[i],2);
		sum=sum + point[i]*item[i];
	}
	meterA=sqrt(meterA);
	meterB=sqrt(meterB);
	sum=sum/(meterA*meterB);
	sum=1-sum;
	if(j==size) return 0;
	return sum;
}

List_nodes_cos* Cosine_input(FILE *fd,int* final_size, int * item){
	int items=0;
	List_nodes_cos *listn=NULL;
    int size=0;
    int tempsize=2;
    float * array;
    char bla[12];
    char c;
    c='t';
    array=malloc(tempsize*sizeof(float));
    fscanf(fd, "%s",bla);
    items++;
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
    List_nodes_cos *tempnod;
    tempnod=malloc(sizeof(List_nodes_cos));
    strcpy(tempnod->point.name,bla);
    memset(bla, 0, sizeof(bla));
    tempnod->point.array=array;
    tempnod->next=listn;
    listn=tempnod;
    int i;
    while(!feof(fd)){
    	fscanf(fd, "%s", bla);
        items++;//	printf("%s",bla);
        if (!strcmp(bla,"")){
        	items--;
            break;
        }
       				
       	tempnod=malloc(sizeof(List_nodes_cos));
       	strcpy(tempnod->point.name,bla);
       	memset(bla, 0, sizeof(bla));
       	tempnod->point.array=malloc(size*sizeof(float));
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

void init_randvector(cos_vec **randvec,int L, int k,int size){
	int i,j;
	(*randvec)=malloc(L*k*(sizeof(cos_vec)));
    for(i=0;i<L*k;i++){
        (*randvec)[i].vector=malloc(size*sizeof(float));
    }
    for(i=0;i<L*k;i++){
        for(j=0;j<size;j++){
        	(*randvec)[i].vector[j]=marsaglia();
        }	
    }

}
void init_hash_cos(List_pointers_cos ****hashtable,cos_vec *randvec,int size,int k,int L,int hashsize,List_nodes_cos *listn,int **G_h){
	int i,j;
	*hashtable=malloc(sizeof(List_pointers_cos **)*hashsize);
	for(i=0;i<hashsize;i++){
		(*hashtable)[i]=malloc(sizeof(List_pointers_cos*)*L);
			for(j=0;j<L;j++){
			(*hashtable)[i][j]=NULL;
		}
	}
//	printf("%d!\n",hashsize);
	printf("Hashtables allocated\n");
	List_nodes_cos *pointer=listn;
	long int bucket;
	long int g;
	while(pointer!=NULL){
	//	printf("%s,%d\n",pointer->point.name,pointer->point.pos);
		for(i=0;i<L;i++){
			bucket=G_cosine(G_h,i, pointer->point, size,k,randvec);
		//	if(i==3 && bucket==2) printf("Item %s G%d, to bucket %d\n",pointer->point.name,i,bucket);
			List_pointers_cos *temptr;
			temptr=malloc(sizeof(List_pointers_cos));
			temptr->nodeptr=&(pointer->point);
			temptr->next=(*hashtable)[bucket][i];
			(*hashtable)[bucket][i]=temptr;
		}
		pointer=pointer->next;
	}
	printf("Data stored in hashtables\n");
}
void search_cosine(List_pointers_cos ***hashtables,FILE *input,List_nodes_cos *listn,int k,int L,int size,cos_vec *randvec,int hashsize,int **G_h,FILE *output){
	int i,j,bucket;
    char radius[20];
	int flag=0;
    fscanf(input, "Radius: %s\n",radius);
   // printf("%s\n",radius);
    double time_spent,time_spent1;
    float Radius= atof(radius);
    float distance, max_distance=2,max_distance1=2;
    List_pointers_cos *neighbor;
    clock_t begin, begin1, end, end1;
 //   printf("%f\n",Radius);
    if (Radius==0) flag=1;
    char bloo[12];
    while(!feof(input)){
    	memset(bloo, 0, sizeof(bloo));
    	fscanf(input, "%s",bloo);
    	if (!strcmp(bloo,"")){
            break;
        }
        Node_cos point;
        point.array=malloc(size*sizeof(float));
        for(i=0;i<size;i++)
        {
        	fscanf(input, "%f", &point.array[i]);
        }
        List_nodes_cos *pointer=listn;
        while(pointer!=NULL){
       			
        		pointer->point.visited=0;
        		pointer=pointer->next;
        		
		}
        max_distance=2;
        begin=clock();
        for(i=0;i<L;i++){
        	bucket=bucket=G_cosine(G_h,i, point, size,k,randvec);
			List_pointers_cos *go=hashtables[bucket][i];
			while(go!=NULL){
			
					if(go->nodeptr->visited==0){
						distance=cosine_distance(point.array,go->nodeptr->array,size);
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
		List_nodes_cos *t_neighbor;
		max_distance1=2;
		begin1=clock();
       	while(pointer!=NULL){
       			
        		distance=cosine_distance(point.array,pointer->point.array,size);
        		if(distance<=max_distance1 && distance!=0){
        			max_distance1=distance;
					t_neighbor=pointer;
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
        	bucket=bucket=G_cosine(G_h,i, point, size,k,randvec);
			List_pointers_cos *go=hashtables[bucket][i];
			while(go!=NULL){
			
					if(go->nodeptr->visited==0){
						distance=cosine_distance(point.array,go->nodeptr->array,size);
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
		fprintf(output,"Nearest neighbor: %s\nDistanceLSH: %f\n",neighbor->nodeptr->name,max_distance);
		fprintf(output,"DistanceTrue: %f\ntLSH: %f\ntTrue:%f\n",max_distance1,time_spent,time_spent1);
		free(point.array);
	
	}
	printf("File written successfully\n");
}

void free_hash_cos(List_pointers_cos  ****hashtable, int hashsize,int L){
	int i,j;
	List_pointers_cos *temp;
	for(i=0;i<hashsize;i++){
		for(j=0;j<L;j++){
			temp=(*hashtable)[i][j];
			while(temp!=NULL){
				List_pointers_cos *temptemp;
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

void free_list_nodes_cos(List_nodes_cos **listn, int size){
	List_nodes_cos *templist;
	int i;
	while((*listn)!=NULL){
		templist=(*listn);
		(*listn)=(*listn)->next;
		free(templist->point.array);
		free(templist);
	}
}

void free_randvec_cos(cos_vec **randvec, int L, int k){
	int i;
	for(i=0;i<L*k;i++){
		free((*randvec)[i].vector);
	}
	free(*randvec);
	(*randvec)=NULL;
}


