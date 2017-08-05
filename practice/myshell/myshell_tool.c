#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>


//内建命令
int com_cd();
int com_help();
int com_quit();
int my_gcwd();
typedef struct {
    char *name;
    rl_icpfunc_t *func;
    char *doc;
} COMMAND;

COMMAND commands[] = {
    { "cd", com_cd, "Change to directory DIR" },
    { "help", com_help, "Display this text" },
    { "exit", com_quit, "Quit using Fileman" },
    { (char *)NULL, (rl_icpfunc_t *)NULL, (char *)NULL }
};
typedef struct {
    int isPip;
    int isInre;
    int isoutre;
    int iner;
    char *com1arg[256];
    char *com2arg[256];
    int background;
    int argnums;
} globalNum;
globalNum globalnums = {0,0,0,0,{},{},0,0};


//readline相关
char*    dupstr(char *s);
void     initialize_readline ();
char**   fileman_completion (const char *text, int start, int end);
char*    command_generator (const char *text, int state);
char*    stripwhite (char *string);
COMMAND  *find_command (char *name);
int execute_line (char *line);
int find_outer(char *name);





int
main(int argc,char *argv)
{
    char *line,*s;
    signal(SIGINT, SIG_IGN);
    initialize_readline();
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    printf("Welcome to \033[34;4mSAO\033[0m ,%s.\n", pwd->pw_name);
    //循环读取。
    while(1){
        my_gcwd();
        line = readline ("\033[34mS\033[35mA\033[33mO\033[0m\033[32m_>:\033[0m ");
        //if (!line){
        //    printf("qwq\n");
        //    break;}
        s = stripwhite (line);
        if (*s)
            {
                add_history(s);
                execute_line(s);
                globalnums.background = 0;
                globalnums.iner = 0;
                globalnums.isoutre = 0;
                globalnums.isPip = 0;
                globalnums.isInre = 0;
                memset(globalnums.com1arg,0,sizeof(globalnums.com1arg));
                memset(globalnums.com2arg,0,sizeof(globalnums.com1arg));

            }

        free(line);
    }
    exit(0);
    return 0;
}

//readline相关
void
initialize_readline ()
{
    rl_readline_name = "SAO_>:";

    rl_attempted_completion_function = fileman_completion;
}


char**
fileman_completion (const char *text, int start, int end)
{
    char **matches;

    matches = (char **)NULL;

    if (start == 0)
        matches = rl_completion_matches (text, command_generator);

    return (matches);
}

char*
command_generator (const char *text, int state)
{
    static int list_index, len;
    char *name;

    if (!state)
        {
            list_index = 0;
            len = strlen (text);
        }

    while (name = commands[list_index].name)
        {
            list_index++;
            if (strncmp (name, text, len) == 0)
                return (dupstr(name));
        }

    return ((char *)NULL);
}

char*
dupstr(char *s)
{
    char *r;

    r = malloc (strlen (s) + 1);
    strcpy(r, s);
    return (r);
}
char*
stripwhite (char *string)
{
    register char *s, *t;
    for (s = string; whitespace (*s); s++)
    if (*s == 0)
        return (s);

    t = s + strlen (s) - 1;
    while (t > s && whitespace (*t))
        t--;

    *++t = '\0';

    return s;
}

int
execute_line (char *line)
{
    char *one = line;
    char *two = line;
    int   number = 0;
    static char arglist[100][256];
    int  count = 0;
    while (1) {
        if ( one[0] == '\0')
            break;
        if ( one[0] == ' ' )
            one++;
        else {
            two = one;
            number = 0;
            while( (two[0] != ' ') && (two[0] != '\0')){
                number++;
                two++;
            }
            strncpy(arglist[count], one, number );
            (arglist[count][number]) = '\0';
            one = two;
            count++;
        }
    }
    //直接用二位数组重写   loading...

    for( int i = 0; i < count; i++) {
        if( strncmp(arglist[i], "&", 1) == 0) {
            if( i == count - 1) {
                globalnums.background = 1;
                count -= 1;
                break;
            } else {
                printf("Wrong arg\n");
                return 0;
            }
        }
    }
    int fstend = 0;
    for( int i = 0; i < count; i++){
        if( !strcmp(arglist[i], ">" )) {
            fstend = i ;
            globalnums.isoutre++;
            globalnums.com2arg[0] = arglist[i+1];
        } else if ( !strcmp(arglist[i], "<")) {
            fstend = i ;
            globalnums.isInre++;
            globalnums.com2arg[0] = arglist[i+1];
        } else if ( !strcmp(arglist[i], ">>")){
            fstend = i ;
            globalnums.iner++;
            globalnums.com2arg[0] = arglist[i+1];
        } else if ( !strcmp(arglist[i], "|")) {
            fstend = i;
            globalnums.isPip++;
            for (int z = 0;i < count-1; i++){
                globalnums.com2arg[z] = arglist[i+1];
                z++;
            }
            break;

        }
    }
    if( fstend == 0){
        fstend =  count;
    }
    globalnums.argnums = fstend;
    for( int d = 0; d < fstend; d++){
        //strcpy(globalnums.com1arg[d], arglist[d]);
        globalnums.com1arg[d] = (char*)arglist[d];
    }
 if( (globalnums.isPip + globalnums.isInre
         + globalnums.isoutre + globalnums.iner)>1){
        printf("error.\n");
    }


    //built-in func.
    COMMAND *command;
    char *word;

    command = find_command(globalnums.com1arg[0]);

    if(command){
        if(globalnums.com1arg[1]==NULL){
            char a[3] ="~";
            globalnums.com1arg[1] = a;
        }
        return ((*(command->func))(globalnums.com1arg[1]));
    } else {
        find_outer(globalnums.com1arg[0]);
    }





    return 0;
}
COMMAND*
find_command (char *name)
{
    register int i;

    for (i = 0; commands[i].name; i++)
        if (strcmp (name, commands[i].name) == 0)
            return (&commands[i]);

    return ((COMMAND *)NULL);
}
int
qwq(char *name){
    DIR *dp;
    struct dirent* dirp;
    int a = 0;
    int c = 0;
    int fff;
    char *path[] = { "/bin","/usr/bin",NULL};
    if( strncmp(name, "./", 2) == 0){
        name = name + 2;
        c++;
    }
    int i = 0;
    while (path[i] != NULL && !c){
        if( (dp= opendir(path[i])) == NULL)
            printf("can not open %s\n" ,path[i]);
        while( (dirp = readdir(dp)) != NULL){
            if ( strcmp(dirp->d_name, name) == 0){
                closedir(dp);
                a++;
                break;
            }
        }
        if (a){
            break;
        }
        i++;
    }
    if( a == 0 && c == 0){
        printf("没有那个命令或文件,请重试\n");
        return 0;
    }
    return 1;
}
int
find_outer(char *name)
{
    if(!qwq(name)) {
        return 0;
    }
    int fff;

    int pid;
    pid = fork();
    switch (pid){
    case 0:{
        if (globalnums.isoutre){
            fff = open(globalnums.com2arg[0], O_WRONLY|O_CREAT|O_TRUNC,0644);
            dup2(fff,1);
        }
        if (globalnums.isInre){
            fff = open(globalnums.com2arg[0], O_RDONLY);
            dup2(fff,0);
        }
        if (globalnums.isPip == 1) {
            fff = open("/home/blind_one/practice/c/group/practice/myshell/chongdingxiang", O_WRONLY|O_CREAT|O_TRUNC,0644);
            dup2(fff,1);
        }
        if (globalnums.iner){
            fff = open(globalnums.com2arg[0], O_WRONLY|O_CREAT|O_APPEND);
            dup2(fff, 1);
        }
        execvp(globalnums.com1arg[0],globalnums.com1arg );
        exit(0);
    }
    default:

        if (globalnums.background){
            printf("Process id %d is runing background\n", pid);
            return 0;
        }
        if (globalnums.isPip == 1){
            waitpid(pid, NULL, 0);
            if(!qwq(globalnums.com2arg[0])){
                printf("error\n");
                return 0;
            }
            int pid2;
            int fd2;
            pid2 = fork();
            switch(pid2){
            case 0:
                fd2 = open("/home/blind_one/practice/c/group/practice/myshell/chongdingxiang",O_RDONLY);
                dup2(fd2,0);
                execvp(globalnums.com2arg[0], globalnums.com2arg);
                exit(0);
            default:
                waitpid(pid2, NULL, 0);
                return 0;
            }
        }
            if (waitpid(pid, NULL, 0) == -1){
                printf("wait for child process error\n");
            }
    }


}
//readline结束。

//内建命令
int
com_cd(char *path)
{
    char a[255];
    char d[255];
    if(!strncmp(path, "~", 1)){
        sprintf(a, "%s%s", "/home/blind_one",path+1);
        chdir(a);
        return 0;
    }
    if(!strncmp(path, "./", 2)){
        path = path + 2;
        chdir(path);
        return 0;

    }
    if(!strncmp(path, "/", 1)){
        chdir (path);
        return 0;
    }
    getcwd(a,sizeof(a));
    sprintf(d, "%s/%s", a,path);
    printf("%s",d);
    chdir(d);
    return 0;
}
int
com_help(char *path)
{
    printf("qwq没有什么特殊操作的啦emmmmm……\n");
    return 0;
}
int
com_quit(char *path)
{
    exit(0);
    return 0;
}

int
my_gcwd()
{
    char path[255],qwq[255],*p;
    getcwd(path, sizeof(path));
    p = path;
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    while(*p != '\0'){
        if (*p == '/'){
            if(!strncmp(p+1, pwd->pw_name, strlen(pwd->pw_name)-1)){
                p += strlen(pwd->pw_name) + 1;
                printf("\033[34m-\033[31m-\033[5m>\033[0m \033[30;44m~\033[0m");
                while (*p != '\0'){
                    printf("\033[30;44m%c\033[0m",*p);
                    p++;
                }
                printf(" \n");
                return 0;
            }
        }
        p++;
    }
    printf ("\033[34m-\033[31m-\033[5m>\033[0m\033[30;44m%s\033[0m\n",path);
    return 0;

}
