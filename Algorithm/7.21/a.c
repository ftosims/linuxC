#include<stdio.h>
#include<stdlib.h>

int
main()
{
    double now, mean;
    for (int i = 0; i < 12; i++){
        scanf("%lf",&now);
        if (i == 0){
            mean = now;
            continue;
        }
        mean += now;
    }
    printf("$%.2lf",mean/12);
}
