#include<iostream>
#include<string>
using namespace std;
int shuzu(int a,int b);
int main (){
    int a,b;
    while(cin>>a>>b){
        shuzu(a,b);
    }
    return 0;
}

int shuzu(int a,int b){
    int qwq[100];
    int c;
    for(int i =0;i<a;i++){
        qwq[i] = (i+1)*2;
    }
    c = a%b;
    int sumqwq=0,assda=0;
    if (a<b){
        goto sss;
    }
    for( assda=0;assda<(a-c);assda++){
        sumqwq +=qwq[assda];
        if (b==1){
            if ((assda+1)!=(a-c))
                cout <<qwq[assda]<<" ";
            else
                cout <<qwq[assda];
            continue;
        }
        if (((assda+1)%b)==0){
            int d;
            d = sumqwq/b;
            if((assda+1)!=(a-c))
                cout << d<<" ";
            else
                cout << d;
            sumqwq = 0;
        }
    }
    sumqwq = 0;
    if (assda == a){
        cout<<endl;
        return 0;
    }
    sss:for (;assda<a;assda++){
        sumqwq += qwq[assda];

    }
    int z = sumqwq /c;
    cout <<z<<endl;
    return 0;
}
