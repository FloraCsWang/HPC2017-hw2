//
//  gs2D-omp.cpp
//  
//
//  Created by Mengran Wang on 3/30/17.
//
//

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include "util.h"
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

int N;

int convertToOneDimension(int x, int y){
    return (N + 2) * x + y;
}


/* compuate global residual, assuming ghost values are updated */
double compute_residual(double * u, int NN, double invhsq){
    int p;
    int q;
    double tmp, res = 0.0;
    #pragma omp parallel for shared(u,NN,invhsq) private(p,q,tmp) reduction(+:res)
    for (p = 1; p <= NN; p++){
        for ( q = 1; q <= NN; q++){
            tmp = ((2.0* u[convertToOneDimension(p,q)]-u[convertToOneDimension(p,q-1)]-u[convertToOneDimension(p,q+1)])*invhsq -1);
            //tmp = ((2.0*u[p][q] - u[p][q-1] - u[p][q+1]) * invhsq - 1);
            res += tmp * tmp;
        }
    }
    return sqrt(res);
    return 0;
}

int main(int argc, const char * argv[]) {
    
    /*
    cout << "input number of thread you want to use \n";
    int threadNum;
    cin >> threadNum;
    omp_set_num_threads(threadNum);
     */

    # pragma omp parallel
    {
    #ifdef _OPENMP
        int my_threadnum = omp_get_thread_num();
        int numthreads = omp_get_num_threads();
    #else
        int my_threadnum = 0;
        int numthreads = 1;
    #endif
        printf("Hello, I'm thread %d out of %d\n", my_threadnum, numthreads);
    }
    
    // insert code here...
    cout << "input number of discretization points N \n";

    cin >> N;
    
    /* timing */
    timestamp_type time1, time2;
    get_timestamp(&time1);
    
    
    double *u = new double[(N+2)*(N+2)]();
    double *unew = new double[(N+2)*(N+2)]();
    int max_iters = 1000;
    
    double h = 1.0 / (N + 1);
    double hsq = h * h;
    double invhsq = 1./hsq;
    double res, res0, tol = 1e-5;
    
    /* initial residual */
    //res0 = compute_residual(u, N, invhsq);
    res0 = 1000;
    res = res0;
    //u[0] = u[N+1] = 0.0;
    int i ;
    int j;
    
    for (int iter = 0; iter < max_iters && res/res0 > tol; iter++) {
       //#pragma omp parallel for default(none) shared(N,unew,u,hsq)
        /* gs step for all the red points */
        for ( i = 1; i <= N;  i++){
            if (i % 2 == 0){
                j = 2;
            }else {
                j = 1;
            }
            #pragma omp parallel for default(none) shared(N,unew,u,hsq, j,i)
            for (int m = j; m <= N; m = m + 2){
                unew[convertToOneDimension(i,m)] = 0.25 *(hsq  + u[convertToOneDimension(i-1,m)] + u[convertToOneDimension(i,m-1)] + u[convertToOneDimension(i+1,m)] + u[convertToOneDimension(i,m+1)] );
            }
            
        }
        
        //#pragma omp parallel for default(none) shared(N,unew,u,hsq)
        /* gs step for all the black points */
        for ( i = 1; i <= N;  i++){
            if (i % 2 == 0){
                j = 1;
            }else {
                j = 2;
            }
            #pragma omp parallel for default(none) shared(N,unew,u,hsq,j,i)
            for (int m = j; m <= N; m = m + 2){
               unew[convertToOneDimension(i,m)] = 0.25 *(hsq  + u[convertToOneDimension(i-1,m)] + u[convertToOneDimension(i,m-1)] + u[convertToOneDimension(i+1,m)] + u[convertToOneDimension(i,m+1)] );
            }
            
        }
        
        
        #pragma omp parallel for default(none) shared(N,unew,u) collapse(2)
        /* copy new_u onto u */
        for ( i= 1 ; i <= N; i++){
            for ( j = 1; j <= N; j++){
                u[convertToOneDimension(i,j)] = unew[convertToOneDimension(i,j)];
            }
        }
        
        if (0 == (iter % 10)) {
            res = compute_residual(u, N, invhsq);
            cout << "Iter : "<<iter<< ", Residual: "<< res<<endl;
        }
    }
    
    
    /* timing */
    get_timestamp(&time2);
    double elapsed = timestamp_diff_in_seconds(time1,time2);
    printf("Time elapsed is %f seconds.\n", elapsed);
    return 0;
    
    
    
}
