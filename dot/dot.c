#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#ifndef N_SIZE
  #define N_SIZE 10
#endif

int a[N_SIZE],b[N_SIZE];

static double time_fill,time_calc,time_gather,time_total;

double mysecond()
{
    struct timeval tp;
    struct timezone tzp;
    int i;

    i = gettimeofday(&tp,&tzp);
    return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

void print_vector(int vec[N_SIZE])
{
  int i;
  for(i=0;i<N_SIZE;i++)
  {
      printf("%d ",vec[i]);                  
  }
  printf("\n");
}
void fill_vector(int vec[N_SIZE])
{
  int i;
  for(i=0;i<N_SIZE;i++)
  {    
      vec[i]=rand_interval(-10,10); 
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
  int dot;

  int sum=0;
  srand(time(NULL));

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  unsigned chunk=N_SIZE/size;
  start=rank*chunk;
  end=((rank+1)*chunk);
  if(rank==0)
  {
    time_fill=mysecond();
    fill_vector(a);
    fill_vector(b);
    time_fill=mysecond()-time_fill;
    //print_vector(a);
    //print_vector(b);
  }
  //int a_partial[chunk],b_partial[chunk];

  MPI_Scatter(a,chunk,MPI_INT,a,chunk,MPI_INT,0,MPI_COMM_WORLD); 
  MPI_Scatter(b,chunk,MPI_INT,b,chunk,MPI_INT,0,MPI_COMM_WORLD);
  if(rank==0)
  {
    time_calc=mysecond();
  }   
  
  for(i=0;i<chunk;i++)
  {
      sum+=a[i]*b[i];
  }
  if(rank==0)
  {
    time_calc=mysecond()-time_calc;
    time_gather=mysecond();
  }
  //MPI_Gather(sum,1,MPI_INT,dot_product[rank],1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Reduce(&sum,&dot,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  if(rank==0)
  {      
    time_gather=mysecond()-time_gather;
    time_total=time_fill+time_gather+time_calc;
    printf("Tarefa %d\n",0);    
    printf("%d\n",dot);
    printf("Fill Calc Reduce Total\n");
    printf("%f %f %f %f\n",time_fill,time_calc,time_gather,time_total);
  }   
  
  MPI_Finalize();
  return 0;
}
