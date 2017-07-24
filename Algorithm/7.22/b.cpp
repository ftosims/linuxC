#include <iostream>
#include <string>

using namespace std;

int
main(void)
{
    int linenums;
    int a,b;
    cin >> linenums;
    cin.get();
    for( int i = 0; i < linenums; i++){
        cin >> a >> b;
        int c;
        if ((c=a+b)>100){
            c = c % 100;
        }
        cout << c << endl;
    }
}
