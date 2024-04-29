#include "Header Files/Math.h"

long pow(int x, int n){
    long x2 = 1;
    for(int i = 0; i<n; i++){
        x2*=x;
    }
    return x2;
}

long rapidpow(int x, int n){
    if(n == 0){
        return 1;
    }
    if(n == 1){
        return x;
    }
    if(n%2 == 0){
        return rapidpow(x*x, n/2);
    }
    return x*rapidpow(x*x, (n-1)/2);
}

//pour faire tourner le cube, on doit calculer des sinus et cosinus
