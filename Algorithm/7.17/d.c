#include<stdio.h>
int main(){
    int i=0,c,b;
    int sum=0;
    while(scanf("%d",&c)!= EOF){
        if (c==0){
            break;
        }
        while (i < c){
            scanf("%d",&b);
            sum +=b;
            i++;
            continue;
        }
        printf("%d\n",sum);
        sum = 0;
        i = 0;
    }
    return 0;
}
