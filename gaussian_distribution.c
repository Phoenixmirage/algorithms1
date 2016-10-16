
//Marsaglia Gaussian

#include <cstdlib> //for rand max 
#include <math.h>
#define RAND_MAX   //This value is implementation dependent. It's guaranteed that this value is at least 32767.

int r,y1,y2,x1,x2;
int M=-1;
int N =1;

y1 = Μ + (rand() / RAND_MAX + 1.0)*(N-M+1);
y2 = Μ + (rand() / RAND_MAX + 1.0)*(N-M+1);
while((y1^2 + y2^2)>=1){
	y1 = Μ + (rand() / RAND_MAX + 1.0)*(N-M+1);
	y2 = Μ + (rand() / RAND_MAX + 1.0)*(N-M+1);
}

r = y1^2 + y2^2;
x1=sqrt(-2*log(r^2)/(r^2))*y1;
x2=sqrt(-2*log(r^2)/(r^2))*y2;			//δεν ξερω αν θες να κρατησουμε το 2ο χ



//NORMAL : int r= N * (rand() / (RAND_MAX + 1.0)); οπου RAND_MAX σταθερη μεταβλητη που συνηθως ειναι ετοιμη