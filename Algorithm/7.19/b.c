#include<stdio.h>
#include<stdlib.h>
int gcd(int a,int b){
    return ((a%b==0?b:gcd(b,a%b)));
}


int main(){
    int num;
    int nums;
    while (scanf("%d",&num)!=EOF){
        long long gcds;
        int i;
        for(i=0;i<num;i++){
            scanf("%d",&nums);
            if (i<1){
                gcds = nums;
                continue;
            }
            int c = gcd (gcds,nums);
            gcds = (gcds/c)*nums;
        }
        printf("%lld\n",gcds);
    }
}
