
//Marsaglia Gaussian
#include <stdio.h>
#include <stdlib.h> //for rand max 
#include <math.h>
#include <time.h>
   //This value is implementation dependent. It's guaranteed that this value is at least 32767.

float marsaglia(void);

int main(){
	srand(time(NULL));
	printf("lets choose 10 random numbers with gaussian distribution\n");
	int i;
	float j;
	
	for(i=1; i<=10; i++){
		j=marsaglia();
		printf("%dnumber: %f\n",i,j);
	}
}

float marsaglia(){
float r,y1,y2,x1,x2;
int M=-1;
int N=1;

y1 = M + (rand() /(RAND_MAX + 1.0)) * (N-M+1);
y2 = M + (rand() /(RAND_MAX + 1.0)) * (N-M+1);

while((pow(y1,2) + pow(y2,2))>=1){
//	printf("%d\n%d\n",y1,y2);
	y1 = M + (rand() /(RAND_MAX + 1.0)) * (N-M+1);
	y2 = M + (rand() /(RAND_MAX + 1.0)) * (N-M+1);
}

r = pow(y1,2) + pow(y2,2);
x1=sqrt(-2*log(pow(r,2))/(pow(r,2)))*y1;
x2=sqrt(-2*log(pow(r,2))/(pow(r,2)))*y2;			

return x1;
}
