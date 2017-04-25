#include "mpi.h"
#include <stdio.h>

int main(int argc,char **argv)
{
	int rank, size,dest,source;	
    int name_len;
    float A[10000],B[10000];
    MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int i;
	for(i=0;i<10000;i++)
	{
		A[i]=rank;
	}	
	dest = (rank+1) % size;
	source = (rank-1)%size;
	
	MPI_Sendrecv(&A,10000,MPI_FLOAT,dest,1,&B,10000,MPI_FLOAT,source,1,MPI_COMM_WORLD,&status);
	//MPI_Recv(&B,10000,MPI_FLOAT,source,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	printf("Tarefa %d recebeu %f\n",rank,B[0]);
	MPI_Finalize();
	return 0;
}
