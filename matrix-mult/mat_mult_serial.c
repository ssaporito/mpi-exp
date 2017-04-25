#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef N_SIZE
	#define N_SIZE 10
#endif

int A[N_SIZE][N_SIZE],B[N_SIZE][N_SIZE],C[N_SIZE][N_SIZE];

static double time_fill,time_calc,time_gather,time_total;

double mysecond()
{
    struct timeval tp;
    struct timezone tzp;
    int i;

    i = gettimeofday(&tp,&tzp);
    return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

void print_matrix(int mat[N_SIZE][N_SIZE])
{
	int i,j;
	for(i=0;i<N_SIZE;i++)
	{
		for(j=0;j<N_SIZE;j++)
		{
			printf("%d ",mat[i][j]);							
		}	
		printf("\n");
	}
	printf("\n");
}
void fill_matrix(int mat[N_SIZE][N_SIZE])
{
	int i, j;
	for(i=0;i<N_SIZE;i++)
	{
		for(j=0;j<N_SIZE;j++)
		{			
			mat[i][j]=rand_interval(-10,10);			
		}			
	}	
}
int rand_interval(int min, int max)
{
    int r;
    const int range = 1 + max - min;
    const int buckets = RAND_MAX / range;
    const int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}


int main(int argc,char **argv)
{ 
	int i,j,k;    
   	int N2=N_SIZE*N_SIZE;   
   	srand(time(NULL));
	

	time_fill=mysecond();
	fill_matrix(A);
	fill_matrix(B);
	time_fill=mysecond()-time_fill;
	//print_matrix(A);
	//print_matrix(B);
	time_calc=mysecond();
		
	for(i=0;i<N_SIZE;i++)
	{
		for(j=0;j<N_SIZE;j++)
		{
			C[i][j]=0;
			for(k=0;k<N_SIZE;k++)
			{
				C[i][j]+=A[i][k]*B[k][j];
			}
		}		
	}
	time_calc=mysecond()-time_calc;
	time_gather=mysecond();
			
	time_gather=mysecond()-time_gather;	
	time_total=time_fill+time_gather+time_calc;	
	//print_matrix(C);
	printf("Fill Calc Gather Total\n");
	printf("%f %f %f %f\n",time_fill,time_calc,time_gather,time_total);
			
	return 0;
}
