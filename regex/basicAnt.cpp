//Basic Ant Colony Algorithm for TSP
#include <iostream>
#include <fstream> 
#include <math.h> 
#include <time.h>
#include <stdlib.h> 
 
using std::cout;
using std::endl;

#define N 14//city size 
#define M 20 //ant number 

double inittao=1; 
double tao[N][N]; 
double detatao[N][N]; 
double distance[N][N]; 
double yita[N][N]; 
int tabu[M][N]; 
int route[M][N]; 
double solution[M]; 
int BestRoute[N]; 
double BestSolution=10000000000; 
double alfa,beta,rou,Q; 

int NcMax; 
void initparameter(void); // initialize the parameters of basic ACA
double EvalueSolution(int *a); // evaluate the solution of TSP, and calculate the length of path
//void InCityXY( double x[], double y[], char *infile ); // input the nodes' coordinates of TSP

void initparameter(void)
{
alfa=1; beta=5; rou=0.9; Q=100;
NcMax=2000; 
}

int main() 
{ 
int NC=0; 
initparameter(); 
double x[N]={16.47,16.47,20.09,22.39,25.23,22.00,20.47,17.20,16.30,14.05,16.53,21.52,19.41,20.09}; 
double y[N]={96.10,94.44,92.54,93.37,97.24,96.05,97.02,96.29,97.38,98.12,97.38,95.59,97.13,92.55}; 


for(int i=0;i<N;i++) 
for(int j=i+1;j<N;j++) 
{ 
distance[j][i]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])); 
distance[i][j]=distance[j][i]; 
}

// calculate the heuristic parameters:
for(i=0;i<N;i++) 
for(int j=0;j<N;j++) 
{ 
tao[i][j]=inittao; 
if(j!=i) 
//yita[i][j]=100/distance[i][j]; //the dividend should be 1 here
yita[i][j]=1/distance[i][j];
}
for(int k=0;k<M;k++) 
for(i=0;i<N;i++) 
route[k][i]=-1; 
srand(time(NULL)); 
for(k=0;k<M;k++) 
{ 
route[k][0]=k%N; 
tabu[k][route[k][0]]=1; 
} 
//each ant try to find the optimal path
do { 
int s=1; 
double partsum; 
double pper; 
double drand; 
//ant choose one whole path
while(s<N) 
{
for(k=0;k<M;k++) 
{ 
int jrand=rand()%3000; 
drand=jrand/3001.0; 
partsum=0; 
pper=0; 

for(int j=0;j<N;j++) 
{ 
if(tabu[k][j]==0) 
partsum+=pow(tao[route[k][s-1]][j],alfa)*pow(yita[route[k][s-1]][j],beta); 

} 
for(j=0;j<N;j++) 
{ 
if(tabu[k][j]==0) 
pper+=pow(tao[route[k][s-1]][j],alfa)*pow(yita[route[k][s-1]][j],beta)/partsum; 
if(pper>drand) 
break; 
} 
tabu[k][j]=1; 
route[k][s]=j; 
} 
s++; 
}

// the pheromone is updated
for(i=0;i<N;i++) 
for(int j=0;j<N;j++) 
detatao[i][j]=0; 

for(k=0;k<M;k++)
{
solution[k]=EvalueSolution(route[k]); 
if(solution[k]<BestSolution) 
{ 
BestSolution=solution[k]; 
for(s=0;s<N;s++) 
BestRoute[s]=route[k][s]; 
} 
}


for(k=0;k<M;k++) 
{ 
for(s=0;s<N-1;s++) 
detatao[route[k][s]][route[k][s+1]]+=Q/solution[k]; 
detatao[route[k][N-1]][route[k][0]]+=Q/solution[k]; 
} 

for(i=0;i<N;i++) 
for(int j=0;j<N;j++) 
{ 
tao[i][j]=rou*tao[i][j]+detatao[i][j]; 
if(tao[i][j]<0.00001) 
tao[i][j]=0.00001; 
if(tao[i][j]>20) 
tao[i][j]=20; 
} 

for(k=0;k<M;k++) 
for(int j=1;j<N;j++) 
{ 
tabu[k][route[k][j]]=0;
route[k][j]=-1; 
} 
NC++; 
} while(NC<NcMax); 

cout<<"the maximum iteration number of ACA is:"<<NcMax<<endl; 
for(i=0;i<N;i++) 
cout<<BestRoute[i]+1<<" ";
cout<<"the shortest length of the path is:"<<BestSolution<<endl; 
} 


double EvalueSolution(int *a) 
{ 
double dist=0; 
for(int i=0;i<N-1;i++) 
dist+=distance[a[i]][a[i+1]];
dist+=distance[a[i]][a[0]];
return dist; 
} 
