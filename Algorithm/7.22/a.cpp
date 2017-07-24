#include <iostream>
#include <string>

using namespace std;

int
main(void)
{
    int linenums;
    string phone_nums,shortnums;
    cin >> linenums;
    cin.get();
    int n = 0;
    for( int i = 0; i < linenums; i++){
        cin >> phone_nums;
        cout << "6" ;
        while (n<5){
            cout << phone_nums[n+6];
            n++;
        }
        n = 0;
        cout << endl;
    }

}
