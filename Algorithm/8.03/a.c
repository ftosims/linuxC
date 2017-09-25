#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    char a[100002], b[100002];
    char *s, *e, *x;
    while(scanf("%s %s", a, b) != EOF) {
        int i;
        for(i = 0; i < 2; i++){

            if(i == 0){
                x = a;
                e = a;
                e += strlen(a)-1;
            } else {
                x = b;
                e = b;
                e += strlen(b)-1;
            }
            while( *x == '0') {
                x++;
            }
            while( *e == '0') {
                e--;
            }
            if(strchr(x, '.') != NULL){
                if( *e == '.') {
                    *e = '\0';
                } else {
                    e++;
                    *e = '\0';
                }
            }
            if( i == 0){
                s = x;
            }
        }
        if(strcmp(s, x) == 0){
            printf("YES\n");
        } else {
            printf("NO\n");
        }

    }
    return 0;

}
