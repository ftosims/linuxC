#include <stdio.h>
#include <stdlib.h>
int main (){
    int thenum[5];
    thenum[0]=153;
    thenum[1]=370;
    thenum[2]=371;

    int group[4];
    thenum[3]=407;
    int maxnum, minnum;
    while (scanf("%d %d",&minnum,&maxnum)!=EOF){
        getchar();
        int count;
        count = 0;
        int gnum = 0;
        for (int i = 0;i<4;i++){
            if (thenum[i]>=minnum&&thenum[i]<=maxnum){
                group[count]=thenum[i];
                
                count ++;
            }
        }
        if (count == 0){
            printf("no\n");
            continue;
        }
        int d = 0;
        for(d = 0; d<count-1;d++){
            printf("%d ", group[d]);
        }
        printf("%d\n", group[d]);
    }
}

