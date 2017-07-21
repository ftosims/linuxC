#include<iostream>
#include<string>

using namespace std;


bool isChinese(char ch){
    int a;
    if (ch<0)
        return true;
    return false;
}
int countIt(string waitForJudge){
    int c;
    int countnum=0;
    for(int i = 0;i<waitForJudge.length();i++){
        char a = waitForJudge[i];
        if (isChinese(waitForJudge[i])){
            countnum++;
        }
    }
    return countnum/3;
}

int main(void){
    int lineNum;
    string judgeOne;
    cin>>lineNum;
    cin.get();
    for(int i=0;i<lineNum;i++){
        getline(cin, judgeOne);
        cout<<countIt(judgeOne)<<endl;
    }
    return 0;
}
