#include "mpi.h"
#include <stdio.h>

int main(int argc,char **argv)
{
	int rank, size;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(processor_name, &name_len);
	printf("Olá, do processador %s,tarefa %d\n",processor_name,rank);
	if(rank==0)
	{
		printf("Há %d tarefas\n",size);
	}
	MPI_Finalize();
	return 0;
}
