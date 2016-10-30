#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef STRUCTS_H

#define STRUCTS_H


typedef struct Node Node;
typedef struct List_nodes List_nodes;
typedef struct List_pointers List_pointers;
typedef struct euc_vec euc_vec;

long int H_euclidean(euc_vec vector_t, Node p,int size,int W);
long int ID_euclidean(int **G_h, int no_G, Node p, int size,long int *random_r, int k,euc_vec *vectors,int W);
int F_Euclidean(long int ID,int hash_size);
float euclidean_distance(float *point, float *item, int size);
List_nodes* Euclidean_input(FILE *fd,int* final_size, int * item);
void init_randvec(euc_vec **randvec,int L, int k,int W,int size);
void init_hash(List_pointers ****hashtable,euc_vec *randvec,int size,int k,int L,int hashsize,List_nodes *listn,int **G_h,int W,long int *random_r);
void search_euclidean(List_pointers ***hashtables,FILE *input,List_nodes *listn,int k,int L,int size,int W,euc_vec *randvec,long int *random_r,int hashsize,int **G_h,FILE*output);

#endif
