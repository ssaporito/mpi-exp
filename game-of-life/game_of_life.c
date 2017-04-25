/***********************

Jogo da Vida
Versão paralela

************************/
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NI 700        /* tamanho dos array  */
#define NJ 700

#define NSTEPS 500    /* Numero de iteracoes */
void print_matrix(int **mat,int start,int end,int rank)
{
  printf("Matriz %d\n",rank);
  int i,j;
  for(i=start;i<=end;i++)
  {
    for(j=1;j<=NJ;j++)
    {
      printf("%d ",mat[i][j]);              
    } 
    printf("\n");    
  }
  printf("\n");
}
void fprint_matrix(int **mat,int start,int end,int rank)
{
  char outfilename[80];
  sprintf(outfilename,"found.data_%d",rank);
  FILE * outfile = fopen(outfilename,"w") ;
  int i,j;
  fprintf(outfile, "Tarefa %d\n",rank);
  for(i=start;i<=end;i++)
  {
    for(j=1;j<=NJ;j++)
    {
      fprintf(outfile,"%d ",mat[i][j]);              
    } 
    fprintf(outfile,"\n");    
  }
  fprintf(outfile,"\n");  
}

int main(int argc, char *argv[]) 
{
  int rank,size,start,end,next,prev,chunk;
  int *displs, *scounts; 
  int i, j, n, im, ip, jm, jp, ni, nj, nsum, isum;
  int **old, **new;  
  float x;
  MPI_Request req[4];
  MPI_Status status[4];

  ni = NI + 2;  /* celulas fantasmas na borda  */
  nj = NJ + 2;

  /* alocacao */
  old = malloc(ni*sizeof(int*));
  new = malloc(ni*sizeof(int*));
  for(i=0; i<ni; i++){
    old[i] = malloc(nj*sizeof(int));
    new[i] = malloc(nj*sizeof(int));
  } 

  /*  inicializando elementos  */
  for(i=1; i<=NI; i++)
  {
    for(j=1; j<=NJ; j++)
    {
       x = rand()/((float)RAND_MAX + 1);
       if(x<0.5){
         old[i][j] = 0;
       } else {
         old[i][j] = 1;
       }
    }
  }
  //print_matrix(old,0,ni-1,-1); 
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  chunk=floor(NI/size);
  start=rank*chunk+1;
  if(rank==size-1)
  {
    end=(NI%size==0)?((rank+1)*chunk):((rank*chunk+1)+(NI%size));
  }
  else
  {
    end=(rank+1)*chunk;
  }
  prev = (rank==0)?(MPI_PROC_NULL):(rank-1);
  next = (rank==size-1)?(MPI_PROC_NULL):(rank+1);
  
  displs = (int *)malloc(size*sizeof(int)); 
  scounts = (int *)malloc(size*sizeof(int)); 
  for (i=0; i<size; ++i) { 
      displs[i] = i*(NJ+2); 
      if(i==size-1)
      {        
        if(NI%chunk!=0)
        {
          scounts[i]=NI%chunk;
        }
        else
        {
          scounts[i]=chunk;
        }        
      }
      else
      {
        scounts[i]=chunk;
      }
  } 

  if(rank==0)
  {                
    //print_matrix(old,1,NI,-1); 
  } 

  MPI_Scatterv(&old[1][1], scounts, displs, MPI_INT,&old[1][1], scounts[rank], MPI_INT,0, MPI_COMM_WORLD); 

  //print_matrix(old,start,end,rank);
  /* */
  for(n=0; n<NSTEPS; n++)
  {
    /* condicoes de controno para as esquinas do dominio */
    old[start-1][0]       = old[end][NJ];
    old[start-1][NJ+1]    = old[end][1];
    old[end+1][NJ+1] = old[start][1];
    old[end+1][0]    = old[start][NJ];

    /* cond. contorno para faces direita/esquerda  */

    for(i=start; i<=end; i++){
      old[i][0]    = old[i][NJ];
      old[i][NJ+1] = old[i][1];
    }

    /* cond. controno face inferior e superior */

    for(j=1; j<=NJ; j++){
      old[start-1][j]    = old[end][j];
      old[end+1][j] = old[start][j];
    }
    /*if(n==0)
    {
    print_matrix(old,start-1,end+1,rank);
    }*/
    // Codigo Paralelo: Trocar elementos da interface paralela    
    MPI_Isend(&old[end][1],NJ,MPI_INT,next,1,MPI_COMM_WORLD,req);
    MPI_Irecv(&old[end+1][1],NJ,MPI_INT,next,1,MPI_COMM_WORLD,req+1);
    
    MPI_Isend(&old[start][1],NJ,MPI_INT,prev,1,MPI_COMM_WORLD,req+2);
    MPI_Irecv(&old[start-1][1],NJ,MPI_INT,prev,1,MPI_COMM_WORLD,req+3);      
    
    MPI_Waitall(4, req, status);

    for(i=start; i<=end; i++)
    {
       for(j=1; j<=NJ; j++)
       {
          im = i-1;
          ip = i+1;
          jm = j-1;
          jp = j+1;

          nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
            + old[im][j ]              + old[ip][j ] 
            + old[im][jm] + old[i][jm] + old[ip][jm];

          switch(nsum)
          {
            case 3:
              new[i][j] = 1;
              break;
            case 2:
              new[i][j] = old[i][j];
              break;
            default:
             new[i][j] = 0;
          }
       }
    }            

    /* copia estado  */
    for(i=start; i<=end; i++){
      for(j=1; j<=NJ; j++){
	       old[i][j] = new[i][j];
      }
    }    
  }


  /*  Conta o número de celulas  vivas no final */
  isum = 0;
  for(i=start; i<=end; i++){
    for(j=1; j<=NJ; j++){
      isum = isum + new[i][j];
    }
  }
  
  //print_matrix(new,start,end,rank);
  
  //printf("k:%d\n",k);
  printf("Tarefa %d de %d\n",rank,size);
  printf("# Celulas Vivas = %d\n", isum);
  
  fprint_matrix(new,start,end,rank); 

  MPI_Finalize();

  for(i=0; i<ni; i++){
    free(old[i]); 
    free(new[i]); 
  }
  free(old);
  free(new);
  
  return 0;
}
