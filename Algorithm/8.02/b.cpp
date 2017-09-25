#include <iostream>
#include <string>

using namespace std;

string qwq[22];
int a,b;
int max;
string letter;
int move[4][2] = { 1,0,
                   0,1,
                   -1,0,
                   0,-1}

void findway(int x, int y);
bool judge(char a);

int
main()
{
    while( cin >> a >> b) {
        for( int i = 0; i < a; i++) {
            cin >> qwq[i];
            cout << qwq[i];
        }
        max = 1
        findway(0,0,1);
    }
}

void
findway(int x, int y,int depth)
{
    for( int i = 0; i < 4; i++){

        char a = string[x][y];
        if(!judge(a)){
            return;
        }
    }
}

bool
judge(char a)
{
    int len = letter.length();
    for( int i = 0; i < len; i++) {
        if (a==letter[i]){
            return false;
        }
    }
    return true
}
