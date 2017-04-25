#include "mpi.h"
#include <stdio.h>

int main(int argc,char **argv)
{
	int rank, size,partner,partner_rank;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Request request;
   	MPI_Status status;
   	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(processor_name, &name_len);
	printf("Olá, do processador %s,tarefa %d\n",processor_name,rank);
	if(rank==0)
	{
		printf("Há %d tarefas\n",size);
	}
	if (rank < size/2)
	{
	 	partner = size/2 + rank;
	}
	else
	{
		partner = rank-size/2;
	}	
	MPI_Isend(&rank,1,MPI_INT,partner,1,MPI_COMM_WORLD,&request);
	MPI_Irecv(&partner_rank,1,MPI_INT,partner,1,MPI_COMM_WORLD,&request);
	MPI_Wait (&request, &status);
	printf("Tarefa %d tem parceria com %d\n",rank,partner_rank);
	MPI_Finalize();
	return 0;
}
