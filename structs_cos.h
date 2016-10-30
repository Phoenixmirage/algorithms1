#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef STRUCTS1_H

#define STRUCTS1_H

typedef struct Node_cos Node_cos;
typedef struct List_nodes_cos List_nodes_cos;
typedef struct List_pointers_cos List_pointers_cos;
typedef struct cos_vec cos_vec;

long int G_cosine(int **G_h, int no_G, Node_cos p, int size, int k,cos_vec *vectors);
int H_cosine(cos_vec vector_t, Node_cos p,int size);
float cosine_distance(float *point, float *item, int size);
List_nodes_cos* Cosine_input(FILE *fd,int* final_size, int * item);
void init_randvector(cos_vec **randvec,int L, int k,int size);
void init_hash_cos(List_pointers_cos ****hashtable,cos_vec *randvec,int size,int k,int L,int hashsize,List_nodes_cos *listn,int **G_h);
void search_cosine(List_pointers_cos ***hashtables,FILE *input,List_nodes_cos *listn,int k,int L,int size,cos_vec *randvec,int hashsize,int **G_h,FILE *output);

#endif
