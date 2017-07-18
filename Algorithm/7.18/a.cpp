#include <iostream>
#include<string>
//验证邮箱格式
using namespace std;
bool IsValidChar(char ch);
bool IsValidEmail(string strEmail);
int main(){
    std::string stremail;
    while (cin>>stremail){
        bool qwq;
        qwq = IsValidEmail(stremail);
        if (qwq){
            cout<<"YES"<<endl;
            continue;
        }
        cout<<"NO"<<endl;
    }
    return 0;
}


bool IsValidEmail(string strEmail)
{
    if( strEmail.length()<5 )
        return false;

    char ch = strEmail[0];
    if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) )
        {
            int atCount =0;
            int atPos = 0;
            int dotCount = 0;
            for(int i=1;i<strEmail.length();i++)
                {
                    ch = strEmail[i];
                    if(IsValidChar(ch))
                        {
                            if(ch==64)
                                {
                                    atCount ++;
                                    atPos = i;
                                }
                            else if( (atCount>0) && (ch==46) )
                                dotCount ++;
                        }
                    else
                        return false;
                }
            if( ch == 46 )
                return false;
            if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
                return false;
            int x,y;
            x=strEmail.find("@.");
            y=strEmail.find(".@");
            if( x>0 || y>0 )
                {
                    return false;
                }
            return true;
        }
    return false;
}
bool IsValidChar(char ch)
{
    if( (ch>=97) && (ch<=122) )
        return true;
    if( (ch>=65) && (ch<=90) )
        return true;
    if((ch>=48) && (ch<=57))
        return true;
    if( ch==95 || ch==45 || ch==46 || ch==64 )
        return true;
    return false;
}
