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
  int i,j,k;   
  int dot=0;
  srand(time(NULL));

  time_fill=mysecond();
  fill_vector(a);
  fill_vector(b);
  time_fill=mysecond()-time_fill;
  //print_vector(a);
  //print_vector(b);

  time_calc=mysecond();
   
  for(i=0;i<N_SIZE;i++)
  {
      dot+=a[i]*b[i];
  }  
  time_calc=mysecond()-time_calc;
  time_gather=mysecond();
         
  time_gather=mysecond()-time_gather;
  time_total=time_fill+time_gather+time_calc;

  printf("%d\n",dot);
  printf("Fill Calc Reduce Total\n");
  printf("%f %f %f %f\n",time_fill,time_calc,time_gather,time_total);

  return 0;
}
