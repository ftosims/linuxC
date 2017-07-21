#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<getopt.h>
#include<string.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>

static const char *optString = "halRr:p:";
struct globalArgs_t{
  char *thepath;           //文件参数保存
  int isIta;               //a
  int isItr;               //r
  int depth;               //迭代深度
  int isIth;               //h
    int hasPath;             //p
  int isItR;               //R
  int isItl;               //l
} globalArgs ;


void printfolderInformation();
int printFilemore(char *path);
void Printthemall(char *path,int depth);
void printfileinformation();

int main(int argc,char *argv[]){
    //初始化参数结构体
    globalArgs.isItl = 0;
    globalArgs.hasPath = 0;
    globalArgs.isIth = 0;
    globalArgs.depth = 0;
    globalArgs.isItR = 0;
    globalArgs.isItr = 0;
    globalArgs.isIta = 0;


    int opt;
    opt = getopt(argc, argv, optString);
    while (opt!= -1){
        switch (opt){
        case 'a':
            globalArgs.isIta = 1;
            break;
        case 'h':
            globalArgs.isIth = 1;
            break;
        case 'R':
            globalArgs.isItR = 1;
            break;
        case 'l':
            globalArgs.isItl = 1;
            break;
        case 'r':
            globalArgs.isItr = 1;
            globalArgs.depth = atoi(optarg);
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
      globalArgs.thepath = "./";
    }

    struct stat typejudge;
    if(stat(globalArgs.thepath, &typejudge)==-1){
        printf("无法获取文件信息，请重试");
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
    Printthemall(globalArgs.thepath,0);
}
void
printfileinformation()
{
    if(globalArgs.isItl)
        printFilemore(globalArgs.thepath);
    printf("%s  ",globalArgs.thepath);
}



int printFilemore(char *path)
{
    char *timeqwq = (char*)malloc(32);
    struct stat buf;
    //地址可能需要再修改。待议
    lstat(path,&buf);
    struct passwd *psd;
    struct group *grp;
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
    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%-8s ", psd->pw_name);
    printf("%-8s ", grp->gr_name);
    //打印文件大小qwq
    printf("%6d ", buf.st_size);
    strcpy(timeqwq, ctime(&buf.st_mtim));
    timeqwq[strlen(timeqwq)-1] = '\0';
    printf(" %s ", timeqwq);
    free(timeqwq);
}

void Printthemall(char *path, int depth)
{
    DIR *d;
    struct dirent *file;
    struct stat sb;
    if(!(d=opendir(path))){
            printf("error opendir %s!\n", path);
            return;
        }
    while((file = readdir(d))!=NULL){
        if (globalArgs.isIta){
            if (strncmp(file->d_name, ".", 2)==0
                ||strncmp("file->name", "..",3)==0)
                continue;
        } else {
            if (strncmp(file->d_name, ".", 1)==0)
                continue;
        }
        if(globalArgs.isItl)
            printFilemore(file->d_name);
        printf("%s  ",file->d_name);
        if(globalArgs.isItl)
            printf("\n");
        if (stat(file->d_name,&sb) >= 0
            && S_ISDIR(sb.st_mode)
            && depth<globalArgs.depth){
            printf("\n");
            Printthemall(file->d_name, depth+1);
        }
    }
    printf("\n");
    closedir(d);
}


