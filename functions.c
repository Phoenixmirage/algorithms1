#include "functions.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float marsaglia(){
	float r,y1,y2,x1,x2;
	int M=-1;
	int N=1;

	y1 = M + ((float)rand() /((float)RAND_MAX + 1.0)) * (N-M+1);
	y2 = M + ((float)rand() /((float)RAND_MAX + 1.0)) * (N-M+1);

	while((pow(y1,2) + pow(y2,2))>=1){
	//	printf("%d\n%d\n",y1,y2);
		y1 = M + ((float)rand() /((float)RAND_MAX + 1.0)) * (N-M+1);
		y2 = M + ((float)rand() /((float)RAND_MAX + 1.0)) * (N-M+1);
	}

	r = pow(y1,2) + pow(y2,2);
	x1=y1*sqrt(-2*log(r))/r;
	x2=y2*sqrt(-2*log(r))/r;			

	return x1;
}

unsigned long long int string_to_int(char *string,int size){
	int i;
	unsigned long long int sum=0,k=1;
	char c;
	for(i=size-1;i>=0;i--){
		c=string[i];
		sum=sum|((c - '0')*k);
	//	printf("%d* %llu\n",(c-'0'),k);
		k=k<<1;
	}
	return sum;
}

void initG_h(int ***G_h, int k, int L, int w, int size){
	int i,j;
	(*G_h)=malloc(sizeof(int*)*L);
    for(i=0;i<L;i++){
       (*G_h)[i]=malloc(sizeof(int)*k);
    }
    int t=0;
	if(w==1){
    	for(i=0;i<L;i++){
    		for(j=0;j<k;j++){
        		(*G_h)[i][j]=t;
        		t++;
        	}
    	}
	}
	else{
		for(i=0;i<L;i++){
        	for(j=0;j<k;j++){
        		t=rand()%size+1;
				(*G_h)[i][j]=t;
        	}
        }
	}
/*	for(i=0;i<L;i++){
        for(j=0;j<k;j++){
        		printf("%d ",(*G_h)[i][j]);
        	}
        printf("\n");
        }*/
}
