#include <stdio.h>
#include <stdlib.h>

int
main()
{
    int n;
    int tech;
    int paper = 0;
    while(scanf("%d", &n)) {
        if(!n)
            return 0;
        int i ;
        for( i = 0; i < n; i++) {
            scanf("%d", &tech);
            if( tech >= 100) {
                paper += tech / 100;
                tech %= 100;
            }
            if( tech >= 50) {
                paper += tech / 50;
                tech %= 50;
            }
            if( tech >= 10) {
                paper += tech / 10;
                tech %= 10;
            }
            if( tech >= 5)  {
                paper += tech / 5;
                tech %= 5;
            }
            if( tech >= 2)  {
                paper += tech / 2;
                tech %= 2;
            }
            if( tech == 1)  {
                paper++;
            }
        }
        printf("%d\n", paper);
        paper = 0;
    }
}
