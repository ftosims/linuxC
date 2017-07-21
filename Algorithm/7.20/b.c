#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
    int j=0,i=0,t=0,d;
    char qwq[1010];
    char k;
    scanf("%d",&d);
    getchar();
    while(d--){
        fgets(qwq, sizeof(qwq), stdin);
        if(qwq[strlen(qwq) - 1] == '\n') qwq[strlen(qwq) - 1] = '\0';
        while(qwq[i]!='\0'){
            while(qwq[i]!=' ')
                {
                    if(qwq[i]=='\0')break;
                    i++;
                }
            t=i-1;
            while(j<t)
                {
                    k=qwq[j];
                    qwq[j]=qwq[t];
                    qwq[t]=k;
                    j++;
                    t--;
                }
            i++;
            j=i;
        }
        i=j=t=0;
        puts(qwq);
    }
    return 0;
}
