#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int a[8][2] = { -1, -2,
                -2, -1,
                 1, -2,
                 2, -1,
                -1,  2,
                -2,  1,
                 1,  2,
                 2,  1};

int b[26][26];
int judge = 0;

int
main()
{

}

int
knight(int x, int y)
{

    int xx, yy;
    for(int i = 0 ; i < 8; i++){
        xx = x + a[i][0];
        yy = y + a[i][1];
        if(!judge(xx,yy)){
            continue;
        }
        knight(xx,yy);
    }
}

int
judge(int xx,int yy)
{
    if(xx < 0|| yy < 0)
        return 0;
    if(b[xx][yy])
        return 0;

}

