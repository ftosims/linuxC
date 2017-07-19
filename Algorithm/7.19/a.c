#include<stdio.h>
#include<stdlib.h>
int cowNum(int years);
int main(){
    int years;
    int cowNums;
    while(scanf("%d",&years)!=EOF){
        if (years==0){
            return 0;
        }
        else
            cowNums = cowNum(years);
        printf("\n%d\n",cowNums);
    }
    return 0;
}
int cowNum(int years){
    if (years<0)
        return 0;
    switch(years){
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 6;
    default:
        return cowNum(years-1)+cowNum(years-3);

    }

}
