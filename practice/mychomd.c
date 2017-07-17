#include<stdio.h>
#include<sys/types.h>
#include <stdlib.h>
#include<sys/stat.h>
#include <unistd.h>


void num_chmod (char **argu);
void char_chmod(char **argu);


int main(int argc,char ** argv[])
{
    if (argv[0] == "sudo"){
        argc--;
        int i;
        for (i = 0;i<argc;i++){
            argv[i] = argv[i+1];
        }
    }
    switch (argc){
        case 3:
            num_chmod(argv);
        case 4:
            char_chmod(argv);
        default:
            printf("%s [mode num] <target>\n",argv[0]);
            printf("or %s [group(can just input owner,group or other.)] [mode(this could input +/- with r,w,x] <target>\n",argv[0] );
            exit(0);
    }
}

void num_chmod(char **argu)
{
    int chmod_num;
    int user_ch;
    int group_ch;
    int other_ch;
    char *path;
    chmod_num = atoi(argu[1]);
    if (chmod_num > 777 || chmod_num < 0){
        printf("mode num error\n");
        exit(0);
    }
    user_ch = chmod_num/100;
    group_ch = (chmod_num - user_ch*100)/10;
    other_ch = chmod_num % 10;
    chmod_num = user_ch*8*8 +group_ch*8 +other_ch;
    path = argu[2];
    if (chmod(path, chmod_num)== -1){
        printf("It didn't work,please try again.\n");
        exit(0);
    }
    exit(0);
}
void char_chmod(char **argu)
{
    struct stat qwqwq;
    char *path;
    path = argu[3];
    int checknum;
    int changemode;
    int modqwq;
    int judge;
    checknum = stat(path, &qwqwq);
    if (checknum == -1){
        printf("error,please try again\n");
        exit(0);
    }
    char *a;
    a = argu[2];
    printf("%s\n %s\n %s\n",&a[0],argu[2],&argu[2][1]);
    if ( atoi(&a[1])== atoi("r"))
        checknum = 4;
    else if (atoi(&a[1])== atoi("w"))
        checknum = 2;
    else if (atoi(&a[1])== atoi("x"))
        checknum = 1;
    else{
        printf("权限指定错误，请检查后重试\n");
        exit(0);
    }
    if (argu[1] = "owner"){
        modqwq = checknum*8*8;
    }
    else if (argu[1] = "group"){
        modqwq = checknum*8;
    }
    else if (argu[1] = "other"){
        modqwq = checknum;
    }
    else{
        printf("用户组有误，请检查后重试\n");
        exit(0);
    }
        if (argu[2][1]=="+"){
            judge = modqwq | qwqwq.st_mode;
        }
        else if (argu[2][1]== "-"){
            judge = modqwq ^ qwqwq.st_mode;
            if (judge== (modqwq | qwqwq.st_mode))
                judge = qwqwq.st_mode;
        }
        if (chmod(path, judge) == 0){
            printf("修改成功\n");
            exit(0);
        }
        printf("修改失败\n");
        exit(0);

}
