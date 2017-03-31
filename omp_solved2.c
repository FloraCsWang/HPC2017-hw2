#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/*
code problem: 1. in the for loop, total operation needs to be atomic, use reduction(+:total)
2. after omp barrier, the team of threads will be synchronized to one. If want to print the current thread number in the last line, need to use omp_get_thread_num(), rather than tid   
*/
int main (int argc, char *argv[]) 
{
int nthreads, i, tid;
float total;

/*** Spawn parallel region ***/
#pragma omp parallel  
  {
  /* Obtain thread number */
  tid = omp_get_thread_num();
  /* Only master thread does this */
  if (tid == 0) {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }

  printf("Thread %d is starting...\n",tid);

  #pragma omp barrier
  /* do some work */
  total = 0.0;
  #pragma omp for schedule(dynamic,10) reduction(+:total)

  for (i=0; i<1000000; i++) 
     total = total + i*1.0;

  printf ("Thread %d is done! Total= %e\n",omp_get_thread_num(),total);

  } /*** End of parallel region ***/
}
