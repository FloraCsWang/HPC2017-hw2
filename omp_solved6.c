#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100
/*code problem: float sum is decalred again in dotprod fuction which makes sum a private varible, and reduction should work on only shared varible and this is also not what we want here
To fix the problem, define sum as a global varible
*/ 

float a[VECLEN], b[VECLEN];

float sum;

float dotprod ()

{
int i,tid;
//float sum;

tid = omp_get_thread_num();
#pragma omp for reduction(+:sum)
  for (i=0; i < VECLEN; i++)
    {
    sum = sum + (a[i]*b[i]);
    printf("  tid= %d i=%d\n",tid,i);
    }
}


int main (int argc, char *argv[]) {
int i;
//float sum;

for (i=0; i < VECLEN; i++)
  a[i] = b[i] = 1.0 * i;
sum = 0.0;

#pragma omp parallel shared(sum)
  dotprod();

printf("Sum = %f\n",sum);
return 0;
}
