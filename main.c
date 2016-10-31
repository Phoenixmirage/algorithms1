#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "structs.h"
#include "structs_cos.h"
#include "structs_ham.h"
#include "structs_ma.h"
#include "functions.h"
#define W 300
int main(int argc, char** argv){
	srand(time(NULL));
	FILE *input, *inputsearch, *output;
	int flagd=1, flagq=1, flago=1,k=4, L=5,i;
	if (argc%2==0 || argc>11){
		printf("something is wrong with the arguments\n");
		return 0;
	}
	char inputstr[50], searchstr[50], outputstr[50];
	for(i=1; i<=argc-2; i=i+2){
		if(!strcmp(argv[i], "-d")){
				strcpy(inputstr,argv[i+1]);
				flagd=0;
		}	
		else if(!strcmp(argv[i], "-q")){
			strcpy(searchstr,argv[i+1]);
			flagq=0;
		}
		else if(!strcmp(argv[i], "-o")){
			strcpy(outputstr,argv[i+1]);
			flago=0;
		}
		else if(!strcmp(argv[i], "-k")){
			k=atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i], "-L")){
			L=atoi(argv[i+1]);
		}
		else{
			printf("Wrong arguments\n");
			return 0;
		}
	}
	if(flagd==1){
		printf("Give input file: ");
		scanf("%s", inputstr);
	}
	printf("Number of hashtables: %d\nNumber of h for each hashtable: %d\n",L,k);
	 if ((input = fopen(inputstr, "r")) == NULL){
     		printf("wrong file name\n");
     		return -1;
		}
	char metric_space[100];
    char metric[100];
    int **G_h;  	//2d array that holds h functions for each G
    int size,j;
    fscanf(input, "@metric_space %s\n", metric_space);
    if (!strcmp(metric_space, "vector")){
		fscanf(input, "@metric %s\n", metric);
		if(!strcmp(metric,"euclidean")){
			int size, items;
			List_nodes *listn;       //list of elements read from file
			listn=Euclidean_input(input,&size, &items); //store elements from file in list
			if(k>(log(items)/log(2))){
				k=(log(items)/log(2))-1;
				printf("k is to big, k is going to be %d\n",k);
			}
			euc_vec *randvec;             // random vectors
			init_randvec(&randvec,L,k,W,size);   
			initG_h(&G_h,k,L,1,size);  
			int hashsize=items/8;       
			List_pointers ***hashtables;   
			long int *random_r;    //1d array with random r
			random_r=malloc(k*sizeof(long int));
			for(i=0;i<k;i++){
				random_r[i]=(long int)rand();
			}
			init_hash(&hashtables,randvec,size,k,L,hashsize,listn,G_h,W,random_r); //store itmes from list to hashtables with euclidean method
			fclose(input);
			char answer[4];
			printf("Want to make a search? (yes/no)\n");
			scanf("%s",answer);
			while(!strcmp(answer,"yes")){
				if(flagq==1){
					printf("Give search file: ");
					scanf("%s", searchstr);
				}
				if(flago==1){
					printf("Give output file: ");
					scanf("%s", outputstr);
				}
				if ((inputsearch = fopen(searchstr, "r")) == NULL){
					printf("wrong file name\n");
					return -1;
				}
				if ((output = fopen(outputstr, "w")) == NULL){
					printf("wrong file name\n");
					return -1;
				}
				printf("Writing to file...\n");
				search_euclidean(hashtables,inputsearch,listn,k,L,size,W,randvec,random_r,hashsize,G_h,output); //search for NN and neighbours in radius
				fclose(inputsearch);
				fclose(output);
				flagq=1;
				flago=1;
				printf("Want to make a search? (yes/no)\n");
				scanf("%s",answer);
			}
			free_randvec(&randvec,L,k);
			free_hash(&hashtables,hashsize,L);
			free_list_nodes(&listn,size);
			freeG_h(&G_h,L);
			free (random_r);
		}
		else{                                                  //vector, not euclidean but cosine
			List_nodes_cos *listn;
			int size, items;
			listn=Cosine_input(input,&size,&items);
			if(k>(log(items)/log(2))){
				k=(log(items)/log(2))-1;
				printf("k is to big, k is going to be %d\n",k);
			}
			cos_vec *randvec;
			init_randvector(&randvec,L,k,size);
			int **G_h;
			initG_h(&G_h,k,L,1,size);
			int hashsize=pow(2,k);
			List_pointers_cos ***hashtables;
			init_hash_cos(&hashtables,randvec,size,k,L,hashsize,listn,G_h);
			fclose(input);
			char answer[4];
			printf("Want to make a search? (yes/no)\n");
			scanf("%s",answer);
			while(!strcmp(answer,"yes")){
				if(flagq==1){
					printf("Give search file: ");
					scanf("%s", searchstr);
				}
				if(flago==1){
					printf("Give output file: ");
					scanf("%s", outputstr);
				}
				if ((inputsearch = fopen(searchstr, "r")) == NULL){
					printf("wrong file name\n");
					return -1;
				}
				if ((output = fopen(outputstr, "w")) == NULL){
					printf("wrong file name\n");
					return -1;
				}
				printf("Writing to file...\n");
				search_cosine(hashtables,inputsearch,listn,k,L,size,randvec,hashsize,G_h,output);
				flagq=1;
				flago=1;
				fclose(inputsearch);
				fclose(output);
				printf("Want to make a search? (yes/no)\n");
				scanf("%s",answer);
			}
			free_randvec_cos(&randvec,L,k);
			free_hash_cos(&hashtables,hashsize,L);
			free_list_nodes_cos(&listn,size);
			freeG_h(&G_h,L);
		}
    }
	else if(!strcmp(metric_space,"hamming")){
		List_nodes_Ham *listn;
		int size, items;
		listn=Hamming_input(input,&size,&items);
		if(k>(log(items)/log(2))){
			k=(log(items)/log(2))-1;
			printf("k is to big, k is going to be %d\n",k);
		}
		int **G_h;
        initG_h(&G_h,k,L,0,size);
        int hashsize=pow(2,k);
        List_pointers_Ham ***hashtables;
        init_hash_Ham(&hashtables,size,k,L,hashsize,listn,G_h);
        fclose(input);
        char answer[4];
        printf("Want to make a search? (yes/no)\n");
        scanf("%s",answer);
        while(!strcmp(answer,"yes")){
        	if(flagq==1){
				printf("Give search file: ");
				scanf("%s", searchstr);
			}	
			if(flago==1){
				printf("Give output file: ");
				scanf("%s", outputstr);
			}
        	if ((inputsearch = fopen(searchstr, "r")) == NULL){
     			printf("wrong file name\n");
     			return -1;
			}
			if ((output = fopen(outputstr, "w")) == NULL){
     			printf("wrong file name\n");
     			return -1;
			}
			printf("Writing to file...\n");
        	search_Ham(hashtables,inputsearch,listn,k,L,size,hashsize,G_h,output);
        	flagq=1;
        	flago=1;
        	fclose(inputsearch);
        	fclose(output);
        	printf("Want to make a search? (yes/no)\n");
        	scanf("%s",answer);
        }	
		free_hash_ham(&hashtables,hashsize,L);
		free_list_nodes_ham(&listn,size);
		freeG_h(&G_h,L);
	}	
	else if(!strcmp(metric_space,"matrix")){
		List_nodes_ma *listn;
		int items,size;
        int **matrix_array;
	listn=matrix_input(input,&size,&items,&matrix_array);
	if(k>(log(items)/log(2))){
				k=(log(items)/log(2))-1;
				printf("k is to big, k is going to be %d\n",k);
			}
        int **G_h;
        initG_h(&G_h,k,L,1,size);
        Dist_points *rand_x;       //array of struct with x1, x2 random variables from matrix and median t1
        rand_x1_x2(matrix_array,&rand_x,L,k,items);
        int hashsize=pow(2,k);
        List_pointers_ma ***hashtables;
        init_hash_ma(&hashtables,matrix_array,rand_x,size,k,L,hashsize,listn,G_h);
        fclose(input);
        char answer[4];
        printf("Want to make a search? (yes/no)\n");
        scanf("%s",answer);
        while(!strcmp(answer,"yes")){
        	if(flagq==1){
				printf("Give search file: ");
				scanf("%s", searchstr);
			}	
			if(flago==1){
				printf("Give output file: ");
				scanf("%s", outputstr);
			}
        	if ((inputsearch = fopen(searchstr, "r")) == NULL){
     			printf("wrong file name\n");
     			return -1;
			}
			if ((output = fopen(outputstr, "w")) == NULL){
     			printf("wrong file name\n");
     			return -1;
			}
			printf("Writing to file...\n");
        	search_matrix(hashtables,matrix_array,rand_x,G_h,k,L,listn,inputsearch,output,size);
       		flagq=1;
        	flago=1;
        	fclose(inputsearch);
        	fclose(output);
        	printf("Want to make a search? (yes/no)\n");
        	scanf("%s",answer);
		}
		free_hash_ma(&hashtables,hashsize,L);
		free_list_nodes_ma(&listn,size);
		free_matrix_array(&matrix_array,size);
		freeG_h(&G_h,L);
		free(rand_x);
	}
}