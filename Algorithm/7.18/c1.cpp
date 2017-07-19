//半成品
#include<iostream>

using namespace std ;

int calc_n_m (int a,int b);

int main(){
    int a,b;
    while (cin>>a>>b){
        calc_n_m(a,b);
    }
    return 0;
}

int calc_n_m(int a,int b){
    int qwq[100];
    int c;
    for(int i=0;i<a;qwq[i]=2*(i+1),i++)
        ;
    ((a%b==0)?(c=0):(c=a%b));
    int sum =0,d;
    for(int i=0;i<a;i++){
        sum += qwq[i];
        ((i==a)?((c)?(b=c,d=1):(d=1)):d=2);
        switch(d){
            case 1:
                printf("%d\n",sum/b);
                break;
            case 2:
                printf("%d ",sum/b);
        }
        ((d==0)?sum==0:sum);
    }
    return 0;
}
