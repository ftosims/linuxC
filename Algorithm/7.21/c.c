#include<stdio.h>
#include<stdlib.h>

int
main()
{
    int lines,thenums;
    int sheeps[33];
    scanf("%d",&lines);
    sheeps[0] = 3;
    for (int a = 1; a < 31; a++){
        sheeps[a] = 2*(sheeps[a-1]-1);
    }
    for (int i = 0; i < lines; i++){
        scanf("%d",&thenums);
        printf("%d\n", sheeps[thenums]);
    }
}
