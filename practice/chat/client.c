#include"setting.h"
#define FILENAME "client.txt"
// include stdio.h by curses.h
//automatically
#include<curses.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
/* for ctrl-C */
#include<signal.h>
/* for inet_aton */
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
/* for usleep */
#include<sys/timeb.h>
#include<time.h>
#include<stdarg.h>
#define IP "127.0.0.1"

//全局变量
//login
char my_id[11];
char cur_id[20];
char my_pwd[11];
char cur_pwd[20];
//网络连接
char IP[25];
int  PORT;
char send_str[250]
//服务端套接字字
int svr_fd;
int clt_fd;



//函数声明
void initial ( void);
void escape ( int a);
//连接服务器
int  cnt_host ( void);
//登录注册
int  logmod ( void);
int  login ( void);
int  zhuce ( void);

//推至serve
ssize_t combsend(int fd , char *msg , unsigned int msg_t , char *format , ... );
//main 登录 | 接收线程 其他处理。
int
main()
{
    signal( SIGINT, escape);
    int i;
    strcpy(my_id, "nobody");
    if( !cnt_host()){
        escape(1);
    }
    //注册登录
    logmod();

}

//登录控制 pre-a
int
logmod()
{
    printf("l to login,r to regist.");
    char a;
    while( a = getchar()) {
        if( a = 'l') {
            if(login())
                break;
        } else if ( a = 'r') {
            zhuce();
        }
    }
    return 1;
}

//login pre-a
int
login()
{
    int a;
    int len;
    char qwq[250];
    printf("Input your nick(no more than 10 letters):");
    scanf("%s%n" , my_id, &a);
    (a>10)?(escape(9)):0;
    for( int i = 0; i < sizeof(my_id); i++) {
        if( my_id[i] == '\n') my_id[i] =  '\0';
    }
    strcpy( cur_id, my_id);
    printf("Input your pwd (no more than 10 letters):\033[08m");
    scanf("%s%n" , my_pwd, &a);
    printf("\033[0m");
    (a>10)?(escape(9)):0;
    for( int i = 0; i < sizeof(my_pwd); i++) {
        if(my_pwd[i] == '\n') my_pwd[i] = '\0';
    }
    strcpy( cur_pwd, my_pwd);
    //推去server认证。
    char pkg[40]
    sprintf(pkg, "%s %s", cur_id, cur_pwd);
    combsend(svr_fd, send_str, sizeof(send_str), "%s %s", LOG,pkg);
    len = recv(svr_fd , qwq , sizeof(qwq) , 0);
    if(len == 0) {
        printf("Connect error,please try again");
        return 0;
    } else if ( !strcmp(qwq, "accept")) {
        printf("Connect success.\n欢迎回来：%s", cur_id);
        return 1;
    } else {
        printf("信息错误，重试");
        return 0;
    }
}
//注册 pre-a
int
zhuce()
{
    char reg_id[11];
    char reg_pwd[11];
    char pkg[40];
    int n;
    printf("hello,please input your nick_name\n(no more than 10 word):");
    scanf("%[a-zA-Z0-9]%n",reg_id, &n);
    if( n == 1 || n > 10) {
        printf("Wrong format.");
        return 0;
    }
    setbuf(stdin, NULL);
    printf("请输入密码\n(Only allow a-z A-Z 0-9)\n(No more than 10 words):\033[8m");
    scanf("%[a-zA-Z0-9]%n",reg_id, &n);
    printf("\033[0m");
    if( n == 1 || n > 10) {
        printf("Wrong format.");
        return 0;
    }
    sprintf(pkg, "%s %s", reg_id, reg_pwd);
    combsend(svr_fd, send_str, sizeof(send_str),"%s %s", REG, pkg);
    len = recv(svr_fd , qwq, sizeof(qwq), 0);
    if(len == 0) {
        printf("Connect error,please try again\n");
        return 0;
    } else if ( !strcmp(qwq, "accept")) {
        printf("Success.\nPlease login.");
        return 1;
    } else {
        printf("Failed.Try again please.\n");
        return 0;
    }

}

//连接服务器 pre-a
int
cnt_host (void)
{
    struct sockaddr_in dest0;

    //get IP addr
    printf("Input IP(enter for default):");
    fgets(IP, sizeof(IP), stdin);
    if( IP[0] == '\n')
        sprintf(IP, "127.0.0.1");
    setbuf(stdin, NULL);
    printf("Input port:");
    scanf("%d" , &PORT);

    //create socket
    svr_fd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&dest0, sizeof(dest0));
    dest0.sin_family = PF_INET;
    dest0.sin_zero = htons(PORT);
    inet_aton(IP, &dest0.sin_addr);

    //connect
    if( connect(svr_fd, (struct sockaddr*)&dest0, sizeof(dest0)) == -1){
        puts("connet( failed)");
        return 0;
    } else { puts("connecting~\n"); }

    return 1;
}



















//推数据。
ssize_t
combsend(int fd , char *msg , unsigned int msg_t , char *format , ... )
{
    va_list arg;
    va_start(arg , format);
    vsnprintf(msg , msg_t , format , arg);
    va_end(arg);
    return send(fd , msg , msg_t , 0);
}
