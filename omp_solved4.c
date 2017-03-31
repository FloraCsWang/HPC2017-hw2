#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 2048

/*code problem: core dumped becasue of stack size is too limited
 * fix the coding by using " ulimit -s 65536" command in unix shell and then call the excutable file 
 */


int main (int argc, char *argv[]) 
{
int nthreads, tid, i, j;
double a[N][N];

/* Fork a team of threads with explicit variable scoping */
#pragma omp parallel shared(nthreads,a) private(tid,i,j)
  {

  /* Obtain/print thread info */
  tid = omp_get_thread_num();
  if (tid == 0) 
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n", tid);

#pragma omp barrier
/* Each thread works on its own private copy of the array */
  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
    a[i][j] = tid + i + j;

  /* For confirmation */
   printf("Thread %d done. Last element= %f\n",tid,a[N-1][N-1]);

  }  /* All threads join master thread and disband */

}
