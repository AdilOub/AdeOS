#include "Header Files/Math.h"

long pow(int x, int n){
    long x2 = 1;
    for(int i = 0; i<n; i++){
        x2*=x;
    }
    return x2;
}