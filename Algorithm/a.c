#include <stdio.h>
#include <string.h>

char all[1100][20];

int main()
{
    int i, j, n;
    int color[1100];
    int cn, max, maxi;
    char b[20];

    while(scanf("%d", &n), n) {
        for(i = 0; i < n; color[i++] = 0);
        cn = 0;
        max = 0;
        for(i = 0; i < n; i++) {
            scanf("%s", b);
            for(j = 0; j < cn; j++) {
                if(strcmp(all[j], b) == 0) {
                    color[j]++;
                    if(color[j] > max) {
                        max = color[j];
                        maxi = j;
                    }
                    break;
                }
            }
            if(j == cn) {
                color[cn]++;
                strcpy(all[cn++], b);
            }
        }
        printf("%s\n", all[maxi]);
    }
    return 0;
}
