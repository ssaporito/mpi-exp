#include "mpi.h"
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
}
void fill_matrix(int mat[N_SIZE][N_SIZE])
{
	int i, j;
	for(i=0;i<N_SIZE;i++)
	{
		for(j=0;j<N_SIZE;j++)
		{
			//printf("%d ",A[i][j]);				
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
	int rank, size,start,end;	
	int i,j,k;    
   	int N2=N_SIZE*N_SIZE;   
   	srand(time(NULL));
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	

	int chunk=N_SIZE/size;
	start=rank*chunk;
	end=(rank+1)*chunk;
	if(rank==0)
	{
		//int A[N_SIZE][N_SIZE],B[N_SIZE][N_SIZE];
		time_fill=mysecond();
		fill_matrix(A);
		fill_matrix(B);
		time_fill=mysecond()-time_fill;
		//print_matrix(A);
		//print_matrix(B);
	}
	
	MPI_Bcast (B,N2, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(A,N2/size,MPI_INT,A[start],N2/size,MPI_INT,0,MPI_COMM_WORLD);	
	if(rank==0)
	{
		time_calc=mysecond();
	}	
	for(i=start;i<end;i++)
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
	if(rank==0)
	{
		time_calc=mysecond()-time_calc;
		time_gather=mysecond();
	}
	MPI_Gather(C[start],N2/size,MPI_INT,C,N2/size,MPI_INT,0,MPI_COMM_WORLD);
	if(rank==0)
	{		
		time_gather=mysecond()-time_gather;
		time_total=time_fill+time_gather+time_calc;
		printf("Tarefa %d\n",0);		
		//print_matrix(C);
		printf("Fill Calc Gather Total\n");
		printf("%f %f %f %f\n",time_fill,time_calc,time_gather,time_total);
	}		
	
	MPI_Finalize();
	return 0;
}
