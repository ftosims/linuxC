#include <stdio.h>
#include <stdlib.h>
using namespace std;

int ut[11];
int dt[11];
int now[11];
int fst[11];
int down[11];

int never(int n);

int
main()
{
    int n;
    int ti = 0;
    while( scanf("%d",&n) != EOF) {
        ti++;
        int nowt = 1;
        int sone = 0;
        for( int i = 0; i < n; i++) {
            scanf("%d %d %d", &ut[i], &dt[i], &now[i]);
            fst[i] = now[i];
            if(now[i] > ut[i]) {
                down[i] = 1;
                sone++;
                now[i] -= ut[i];
            } else {
                down[i] = 0;
            }
        }
        while(1) {
            if(sone == 0)
                break;
            int t = 0;
            for( int i = 0; i < n; i++) {
                ++now[i];
                if(down[i]){
                    if(now[i] > dt[i]) {
                        now[i] = 1;
                        down[i] = 0;
                        t--;
                    }
                } else {
                    if( now[i] > ut[i]) {
                        now[i] = 1;
                        if(sone*2 > n) {
                            down[i] = 1;
                            t++;
                        }
                    }
                }
            }
            sone += t;
            if(never(n)) {
                nowt = -1;
                break;
            }
            ++nowt;
        }
        printf("Case %d: %d\n", ti, nowt);
    }
}

int
never(int n)
{
    for(int i = 0; i < n; i++) {
        int t = 0;
        if(down[i])
            t = ut[i];
        if(now[i] + t != fst[i])
            return 0;
    }
    return 1;
}
