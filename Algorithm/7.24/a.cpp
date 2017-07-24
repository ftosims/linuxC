#include <iostream>
#include <string>
#include <cmath>

using namespace std;
int B,u,l,b,a,s,r;
int
main(void)
{
    string judgeone;
    while (getline(cin,judgeone)){
        B = u = l = b = a = s = r = 0;
        int len = judgeone.length();
        for (int i = 0; i < len; i++){
            if (judgeone[i] == 'B')B++;
            if (judgeone[i] == 'u')u++;
            if (judgeone[i] == 'l')l++;
            if (judgeone[i] == 'b')b++;
            if (judgeone[i] == 'a')a++;
            if (judgeone[i] == 's')s++;
            if (judgeone[i] == 'r')r++;
        }
        u = u/2;
        a = a/2;
        if (B <= u && B <= l && B <= b &&B <= a &&B <= s &&B <= u && B <= r)
                    cout << B << endl;
        else{
            int minnums;
            minnums = min(B,u );
            minnums = min(minnums,l);
            minnums = min(minnums,b);
            minnums = min(minnums,a);
            minnums = min(minnums,s);
            minnums = min(minnums,r);
            cout << minnums << endl;
        }
    }
    return 0;
}
