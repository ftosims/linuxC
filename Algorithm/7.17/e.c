#include<stdio.h>
int main(){
    int i=0,c,b,a;
    int sum=0;
    scanf("%d",&a);
    while(scanf("%d",&c)!= EOF && a>0){
        while (i < c){
            scanf("%d",&b);
            sum +=b;
            i++;
            continue;
        }
        printf("%d\n",sum);
        sum = 0;
        i = 0;
        a--;
    }
    return 0;
}
