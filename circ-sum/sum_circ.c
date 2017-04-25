#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	int rank, size,dest,source;	
    int name_len;
    float A,B,SUM;
    SUM=0;
    MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	
	A=rank;
	dest = (rank+1) % size;
	source = (rank-1)%size;
	int i;	
	for(i=0;i<size;i++)
	{
		MPI_Sendrecv(&A,1,MPI_FLOAT,dest,1,&B,1,MPI_FLOAT,source,1,MPI_COMM_WORLD,&status);
		SUM+=B;
		A=B;
	}
	//MPI_Recv(&B,10000,MPI_FLOAT,source,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	printf("Tarefa %d tem soma: %f\n",rank,SUM);
	MPI_Finalize();
	return 0;
}
