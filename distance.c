#include "distance.h"

n=1000;
k=4;
L=5;
int search[n];
float distances[n][n],temp[n];
int ran1,ran2,k,h,table_size,g_dec,pos;
float h_i;
float t1;
char h;
char g_form[L][k];
int g[L];

for (f=0;f<L;f++){
	i=1;
	float sum=0;
	int count =0;
	ran1 = rand() % n;
	ran2= rand() % n;
	while(ran1==ran2)
		ran2= rand() % n;
	for (int j=1; j++ ;j<=n){
		h_i=((distances[j][ran1]^2)+(distances[j][ran2]^2)-(distances[ran1][ran2]^2))/(2*distances[ran1][ran2]);
		sum+=h_i;
		count++;
	}
	t1=sum/count;																	//average needed for creating the hi variables
	
	while(i<=k){
		x=f*k+i-1;
		ran1 = rand() % n;
		ran2= rand() % n;
		while(ran1==ran2)
			ran2= rand() % n;
		h_i=((distances[x][ran1]^2)+(distances[x][ran2]^2)-(distances[ran1][ran2]^2))/(2*distances[ran1][ran2]);
		if(h_i>=t1)
			h="1";
		else
			h="0";
		g_form[f][i-1]=h;
		i++;
	}
	g[f]=atoi(g_form);
}

table_size=2^k;
node_t htable[table_size][L];

node_t * head = NULL;
head = malloc(sizeof(node_t));
head->flag = 0;
head->next = NULL;



for(i=0 ; i<n ;i++){

	for(j=0;j<L;j++){
		g_dec=0;								//decimal form of gi
		for(s=0;s<k;s++){
			g_dec+=g_form[j][s]*2^s;
		}
		pos = i % g_dec;						//there the hash function sends the item

		for(b=0;b<n;b++){
			temp[b]=distances[i][b];
		}

		node_t * current = htable[pos][j];		
		if(current!=NULL){						//if list is not empty ,trace to the end and add the node
		    while (current->next != NULL) {
		        current = current->next;
		    }
		    current->next = malloc(sizeof(node_t));
		    current->next->dist = temp;
		    current->next->flag = 0;
		    current->next->next = NULL;
		}
		else{
			current = malloc(sizeof(node_t));
			current->next->dist = temp;
			current->flag = 0;
			current->next=NULL;
		}

	}
}

q=100;
float search[q][n];
float temp1[n];
int flag,t;
for(i=0;i<q;i++){						//for each item in search 
	flag=0;
	for(k=0;k<L;k++){					//search to find it in the hash table
		j=0
		while(j<table_size && flag==0){
			temp1=htable[j][k]->dist;
			htable[j][k]->flag=1;
			flag=1;
			t=0;	
			while(flag==1 && t<n){
				if(search[i][t]!=temp[t])
					flag=0;
				t++;
			}
			if(flag==1){
				
			}


			j++;
		}
	}
}
