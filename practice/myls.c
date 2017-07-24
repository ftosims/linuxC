#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>

static const char *optString = "halRp:";
struct globalArgs_t{
    char *thepath;           //文件参数保存
    int isIta;               //a
    // int isItr;               //r
    int isIth;               //h
    int hasPath;             //p
    int isItR;               //R
    int isItl;               //l
} globalArgs ;

void printfolderInformation();
int printFilemore(char *path);
void Printthemall(char *path);
void printfileinformation();
void helpfunc();
void recursiondir(char *path);

int main(int argc,char *argv[]){
    //初始化参数结构体
    globalArgs.isItl = 0;
    globalArgs.isIth = 0;
    globalArgs.isItR = 0;
    globalArgs.isIta = 0;
    globalArgs.hasPath = 0;


    int opt;
    opt = getopt(argc, argv, optString);
    while (opt!= -1){
        switch (opt){
        case 'a':
            globalArgs.isIta = 1;
            break;
        case 'h':
            globalArgs.isIth = 1;
            helpfunc();
            break;
        case 'l':
            globalArgs.isItl = 1;
            break;
        case 'R':
            globalArgs.isItR = 1;
            break;
        case 'p':
            globalArgs.hasPath = 1;
            globalArgs.thepath = optarg;
            break;
        default:
            printf("Wrong argument %c.",opt);
            exit(1);
        }
        opt = getopt(argc, argv, optString);
    }
    if (!globalArgs.hasPath){
      globalArgs.thepath = ".";
    }

    struct stat typejudge;
    if(lstat(globalArgs.thepath, &typejudge)==-1){
        printf("无法获取文件信息，请重试");
        exit(0);
    } else if( globalArgs.isItR ){
        recursiondir( globalArgs.thepath );
        exit(0);
    } else if(S_ISREG(typejudge.st_mode)){
        printfileinformation();
    } else {
        printfolderInformation();
    }

    return 0;
}

void
printfolderInformation()
{
    Printthemall(globalArgs.thepath);
}
void
printfileinformation()
{
    if(globalArgs.isItl)
        printFilemore(globalArgs.thepath);
}



int printFilemore(char *path)
{
    char *timeqwq = (char*)malloc(32);
    struct stat buf;
    //地址可能需要再修改。待议
    lstat(path,&buf);
    struct passwd *owner;
    struct group *group;
    //文件类型判断
    if (S_ISLNK(buf.st_mode))
        printf("l");
    else if (S_ISREG(buf.st_mode))
        printf("-");
    else if (S_ISDIR(buf.st_mode))
        printf("d");
    else if (S_ISCHR(buf.st_mode))
        printf("c");
    else if (S_ISBLK(buf.st_mode))
        printf("b");
    else if (S_ISFIFO(buf.st_mode))
        printf("f");
    else if (S_ISSOCK(buf.st_mode))
        printf("s");
    //权限判断
    ((buf.st_mode&S_IRUSR)?(putchar('r')):(putchar('-')));
    ((buf.st_mode&S_IWUSR)?(putchar('w')):(putchar('-')));
    ((buf.st_mode&S_IXUSR)?(putchar('x')):(putchar('-')));
    ((buf.st_mode&S_IRGRP)?(putchar('r')):(putchar('-')));
    ((buf.st_mode&S_IWGRP)?(putchar('w')):(putchar('-')));
    ((buf.st_mode&S_IXGRP)?(putchar('x')):(putchar('-')));
    ((buf.st_mode&S_IROTH)?(putchar('r')):(putchar('-')));
    ((buf.st_mode&S_IWOTH)?(putchar('w')):(putchar('-')));
    ((buf.st_mode&S_IXOTH)?(putchar('x')):(putchar('-')));
    printf("  ");
    //打印文件链接数
    printf("%4d ", buf.st_nlink);
    //获取并打印所有者用户名和组名
    owner = getpwuid(buf.st_uid);
    group = getgrgid(buf.st_gid);
    printf("%-11s ", owner->pw_name);
    printf("%-11s ", group->gr_name);
    //打印文件大小qwq
    printf("%6d ", buf.st_size);
    strcpy(timeqwq, ctime(&buf.st_mtim));
    timeqwq[strlen(timeqwq)-1] = '\0';
    printf(" %s ", timeqwq);
    free(timeqwq);
}

void Printthemall( char *path )
{
    DIR *d;
    struct dirent *file;
    struct stat sb;
    char qwq[1024];
    if(!(d=opendir(path))){
            printf("error opendir %s!\n", path);
            return;
    }
    while((file = readdir(d))!=NULL){
        sprintf(qwq, "%s/%s",path,file->d_name);
        if (globalArgs.isIta){
        } else {
            if (strncmp(file->d_name, ".", 1)==0)
                continue;
        }
        if(globalArgs.isItl){
            printFilemore(qwq);
        }
        printf("%s  ",file->d_name);
        if(globalArgs.isItl)
            printf("\n");
    }
    printf("\n");
    closedir(d);
}

void
helpfunc()
{
    printf("请输入-a以显示隐藏文件，-l以显示详细信息，-R显示迭代文件夹");
    exit(0);
}

void
recursiondir(char *path)
{
    DIR *d;
    struct dirent *file;
    struct stat sb;
    char qwq[1024];
    if(!(d=opendir(path))){
        printf("error opendir %s!\n", path);
        return;
    }
    while((file = readdir(d))!=NULL){
        sprintf(qwq, "%s/%s",path,file->d_name);
        if( globalArgs.isIta){
        } else {
            if ( !strncmp( file->d_name, ".", 1))
                 continue;
        }
        if( globalArgs.isItl)
            printFilemore(qwq);
        printf( "%s  ", file->d_name );
        if( globalArgs.isItl )
            printf("\n");
    }
    printf("\n");
    closedir(d);
    if(!(d=opendir(path))){
        printf("error opendir %s!\n", path);
        return;
    }
    while((file = readdir(d))!=NULL){
        sprintf(qwq, "%s/%s",path,file->d_name);
        if( globalArgs.isIta){
            if( !strcmp( file->d_name, ".")
                || !strcmp( file->d_name, ".."))
                continue;
        } else {
            if ( !strncmp( file->d_name, ".", 1))
                continue;
        }
        if( lstat(qwq, &sb ) >= 0 && S_ISDIR(sb.st_mode) ){
            printf("%s :\n", qwq);
            recursiondir(qwq);
        }
    }
    closedir(d);
}
