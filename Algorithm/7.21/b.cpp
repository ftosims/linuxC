#include<iostream>
#include<string>
using namespace std;
int
main(void)
{
    int linenums;
    string inchar;
    cin >> linenums;
    cin.get();
    for (int i = 0; i < linenums; i++){
        cin >> inchar;
        for (int s = 0, e = inchar.length()-1;
             s < e ;s++,e--){
            if (inchar[s] == inchar[e]
                && (s == e -2 || s == e - 1)){
                cout << "yes"<<endl;
                break;
            } else if (inchar[s]!= inchar[e]){
                cout << "no"<<endl;
                break;
            }
        }
    }
    return 0;
}
