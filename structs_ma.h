#ifndef STRUCTS5_H

#define STRUCTS5_H
#include <stdio.h>
typedef struct Node_ma Node_ma;
typedef struct Dist_points Dist_points;
typedef struct List_nodes_ma List_nodes_ma;
typedef struct List_pointers_ma List_pointers_ma;

List_nodes_ma* matrix_input(FILE *fd,int* final_size, int * item,int ***array);
void rand_x1_x2(int **array,Dist_points **rand_x, int L, int k, int size);
void init_hash_ma(List_pointers_ma ****hashtable,int **array,Dist_points *rand_x,int size,int k,int L,int hashsize,List_nodes_ma* listn,int **G_h);
long int G_matrix(int **array,Dist_points *rand_x, Node_ma point, int **G_h,int k,int no_G);
int H_matrix(int **array,Dist_points rand_x,Node_ma point);
void free_matrix_array(int ***array,int size);
void free_list_nodes_ma(List_nodes_ma **listn, int size);
void free_hash_ma(List_pointers_ma  ****hashtable, int hashsize,int L);
void search_matrix(List_pointers_ma ***hashtable, int **array, Dist_points *rand_x, int **G_h, int k, int L, List_nodes_ma *listn,FILE *input,FILE *output,int size);
#endif

