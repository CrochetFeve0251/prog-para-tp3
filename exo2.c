#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int x = 0;
int y = 0;
int sum(float * a, int n){
    int m = n/2;
    if(n==0){
        return 0;
    }
    if(n==1){
        return *a;
    }

    #pragma omp task
        x = sum(a, m);

    #pragma omp task
        y = sum(a+m, n-m);

    #pragma omp taskwait
        x = x+y;

    return x;
}

int main (){
    float t = 5;
    float * a = &t;
    int n = 10;
    #pragma omp single
        int total = sum(a,n);

    return 0;
}
