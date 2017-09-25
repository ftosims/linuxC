#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int
main()
{
    int n;
    int base;
    long long first,mid;
    long long second;
    for( scanf("%d %d", n, base) ！= EOF){
        for( int i = 0; i < n; i++){
            scanf("%d",mid);
            first += mid * pow(10, i);
        }
    }
    
}
