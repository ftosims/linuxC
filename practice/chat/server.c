#include"def.h"
#define FILENAME "server.txt"
/* add stdio.h automatically */
#include<curses.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
#include<dirent.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
/* for usleep */
#include<sys/timeb.h>
#include<time.h>
#include<mysql/mysql.h>

//sql
#define SHOST    "localhost"
#define SUSER    "root"
#define SPASSWD  "wjlshishen"
#define SDB_NAME "chatinfo"


ssize_t combsend(int fd , char *msg , unsigned int msg_t , char *format , ... );

//socket相关
int sockfd , clientfd , svr_port , ch , online = 0 , curline=0;
char g_sd_str[307];
void recv_msg(void *);
int  sendtoaim(int mod, char *recvname, int sendfd, char *string);

//MySQL
MYSQL *mysql;
MYSQL *offsql;
void mysql_connect( MYSQL *mysql);
int  judge_group( char *obj);
int  judgein( int mod, char *target, char *messager);
int  ifonline( char *name);
void addfriend(char *a, char *b, int sendfd);
int  offline(char *recer, char *string);
int  whenoffline(char *name, int fd);
int grptalk(char *grpname, char *string);
int list( int mod, char *name, int fd);

struct clientinfo {
    pthread_t id;//for thread to recv_msg
    int root , fd;
    char name[20] , curname[20];//curname = [root | self.name]
} client[30]={ [0 ... 30-1]={ .fd=-1 , .root=0 } };

int
main()
{

    int i, len;

    struct sockaddr_in chatroom_addr, client_addr;
    printf("Input serve PORT:");
    scanf("%d", &svr_port);

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(!(~sockfd))
        printf("Socket creating failed.\n"),exit(1);

    bzero(&chatroom_addr, sizeof(chatroom_addr));
    chatroom_addr.sin_family = PF_INET;
    chatroom_addr.sin_port   = htons(svr_port);
    chatroom_addr.sin_addr.s_addr = INADDR_ANY;

    if(!(~bind(sockfd, (struct sockaddr*)&chatroom_addr, sizeof(chatroom_addr))))
        printf("bind failed\n"),exit(1);

    listen(sockfd, 30);

    mysql = mysql_init(NULL);
    mysql_connect(mysql);
    offsql = mysql_init(NULL);
    mysql_connect(offsql);

    printf("wait connecting.......");

    while(1) {
        for( i = 0; i < 30; i++)
            if(client[i].fd == -1) break;

        len = sizeof(client_addr);
        client[i].fd = accept(sockfd, (struct sockaddr*)&client_addr, &len);
        if( online == 29) {
            combsend(client[i].fd, g_sd_str, sizeof(g_sd_str), "服务器爆满qwq" ,CMD_MSG);
            close(client[i].fd), client[i].fd = -1;
        } else {
            if(pthread_create(&client[i].id, NULL, (void *)recv_msg, (void*)i)) {
                printf("create client[%d] thread failed\n",i);
                client[i].fd = -1;
            }
        }
    }
    return 0;
}

void
mysql_connect(MYSQL *mysql)
{
    if(!mysql_real_connect(mysql, SHOST, SUSER, SPASSWD, SDB_NAME, 0, NULL, CLIENT_MULTI_STATEMENTS)) {
        printf("Failed to connect:%s\n", mysql_error(mysql));
    }
    printf("Connect database sucessfully!\n\n");
}

ssize_t combsend(int fd , char *msg , unsigned int msg_t , char *format , ... ){
    va_list arg;
    va_start(arg , format);
    vsnprintf(msg , msg_t , format , arg);
    va_end(arg);
    printf("%s\n", msg);
    return send(fd , msg , msg_t , 0);
}

void
recv_msg(void *num)
{
    int endsub = 0, idx = (int)num, len, i;
    char msg_str[310], g_cmd[10], man[20], string[320] = "";
    online++;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL);
    //OFFLINE 消息发送。好友 群聊。
    while(1) {
        len = recv(client[idx].fd, msg_str, sizeof(msg_str), 0);
        if(!len){
            printf("client[%d] connection break\n", idx);
            curline++;
            close(client[idx].fd);
            client[idx].fd = -1;
            break;
        } else if(len > 0) {
            int readed = 0, ch_cnt = 0;
            g_cmd[0] = '\0';
            sscanf(msg_str, "%s%n", g_cmd, &readed);
            printf("recv command[%s] from %d\n", msg_str, idx);
            if(!strcmp(g_cmd, GRP_MSG)) {
                //群聊
                char grp[11];
                char sendone[250];
                sscanf(msg_str+readed, "%s%n", grp, &ch_cnt);
                readed += ch_cnt;
                //判断群是否存在。未写。
                if(!judgein(1, grp, client[idx].name)){
                    //不是群员;
                    char waring[30] = "You are not in this group.";
                    sendtoaim(3, client[idx].name, client[idx].fd,waring);
                    continue;
                }
                //是群员；
                sprintf(sendone, "%s_%s:%s", grp,client[idx].name, msg_str+readed);
                grptalk(grp, sendone);
            } else if(!strcmp(g_cmd, PRV_MSG)) {
                //私聊 需要好友判断。
                char fris[11];
                sscanf(msg_str+readed, "%*[ ]%s%n", fris,&ch_cnt);
                readed += ch_cnt;
                if(!judgein(2, fris, client[idx].name)){
                    char fwaring[100] = "you are not friends.";
                    //不是好友;
                    sendtoaim(3, client[idx].name, client[idx].fd, fwaring);
                    memset(fwaring, '\0', sizeof(fwaring));
                    continue;
                }
                //是好友
                int fd;
                char sendone[250];
                sprintf(sendone, "(%s to %s)>%s", client[idx].name, fris, msg_str+readed);
                // 发 送
                sendtoaim(2, fris, client[idx].fd, sendone);
            } else if(!strcmp(g_cmd, ADD_MSG)) {
                cha name[12];
                int  a;
                sscanf(msg_str+readed, "%s%n", name, &a);
                addfriend(client[idx].name, name, client[idx].fd);
            } else if(!strcmp(g_cmd, CREATEG)) {
                //创建群
                MYSQL_RES *result;
                MYSQL_ROW  row;
                char grp[11];
                sscanf(msg_str+readed, "%s", grp);
                char grpquery[100];
                memset(grpquery, '\0', sizeof(grpquery));
                sprintf(grpquery, "INSERT INTO groupid (name) VALUES ('%s');", grp);
                printf("%s",grpquery);
                if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
                    printf("%s\n",mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    sendtoaim(3, client[idx].name, client[idx].fd,"ERROR.");
                    continue;
                }
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));
                time_t t;
                struct tm *tblock;
                t = time( NULL);
                tblock = localtime(&t);
                memset(grpquery, '\0', sizeof(grpquery));
                sprintf(grpquery,"INSERT INTO syslog (timedate, log) VALUES ('%s','%s has create %s.');", asctime(tblock),client[idx].name, grp);
                if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
                    printf("%s\n",mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    continue;
                }
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));
                sendtoaim(3, client[idx].name, client[idx].fd,"Create complete.");
            } else if(!strcmp(g_cmd, JOIN_MS)) {
                MYSQL_RES *result;
                MYSQL_ROW  row;
                char grp[12];
                sscanf(msg_str+readed, "%s", grp);
                if(judgein(1,grp, client[idx].name)) {
                    //是群员
                    char fwaring[100] = "You have already been in that group.";
                    sendtoaim(3, client[idx].name, client[idx].fd, fwaring);
                    continue;
                }
                //添加操作
                char grpquery[100];
                memset(grpquery, '\0', sizeof(grpquery));
                sprintf(grpquery,"INSERT INTO groupls (gid,memberid) VALUES ('%s', '%s');", grp, client[idx].name );
                if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
                    printf("%s\n",mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    sendtoaim(3, client[idx].name, client[idx].fd,"ERROR.");
                    continue;
                }
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));

                time_t t;
                struct tm *tblock;
                t = time( NULL);
                tblock = localtime(&t);
                memset(grpquery, '\0', sizeof(grpquery));
                sprintf(grpquery,"INSERT INTO syslog (timedate, log) VALUES ('%s','%s has join %s.');", asctime(tblock),client[idx].name, grp);
                if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
                    printf("%s\n",mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    sendtoaim(3, client[idx].name, client[idx].fd,"ERROR.");
                    continue;
                }
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));
                sendtoaim(3, client[idx].name, client[idx].fd,"Join complete.");
            } else if(!strcmp(g_cmd, DEL_MSG)) {

                MYSQL_RES *result;
                MYSQL_ROW  row;
                char delone[20];
                sscanf(msg_str+readed, "%*[ ]%s%n", delone,&ch_cnt);
                readed+= ch_cnt;
                char delquery[200];
                sprintf( delquery, "DELETE FROM friends WHERE (a = '%s' and b = '%s') or(a = '%s' and b = '%s');", client[idx].name, delone,delone,client[idx].name);
                if( mysql_real_query(mysql, delquery, sizeof(delquery))) {
                    printf("%s", mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    continue;
                }
                sendtoaim(3, client[idx].name, client[idx].fd,"DEL complete.");
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));
                time_t t;
                struct tm *tblock;
                char grpquery[200];
                t = time( NULL);
                tblock = localtime(&t);
                memset(grpquery, '\0', sizeof(grpquery));
                sprintf(grpquery,"INSERT INTO syslog (timedate, log) VALUES ('%s','%s del %s from its ls.');", asctime(tblock), client[idx].name, delone);
                if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
                    printf("%s\n",mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    continue;
                }
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));
            } else if(!strcmp(g_cmd, CMD_MSG)) {
                char aim[5];
                int  a;
                sscanf( msg_str+readed, "%s%n", aim, &a);
                readed += a;
                if (!strcmp(aim, "fa")){
                    //全部好友
                    list(1, client[idx].name, client[idx].fd);
                } else if (!strcmp(aim, "fo")) {
                    //在线好友
                    list(2, client[idx].name, client[idx].fd);
                } else if (!strcmp(aim, "gl")) {
                    char grpn[13];
                    sscanf( msg_str+readed,"%s", grpn);
                    list(3,grpn,client[idx].fd);
                }
            } else if(!strcmp(g_cmd, "FILE")) {
                int recvffd = 0;
                int method;
                char recvname[20];
                sscanf(msg_str,"%*s %s", recvname);
                if(( recvffd = ifonline(recvname)) != -1) {
                    method = 1;
                }
                int i;
                if(method = 1) {
                    combsend(recvffd, g_sd_str, sizeof(g_sd_str),"%s", msg_str);
                    printf("%s", msg_str);
                } else {
                    offline(recvname ,string);
                }
            } else if(!strcmp(g_cmd, LOG_MSG)) {
                //登录
                int rtn = 0;
                char pwdc[20];
                char sqlqwq[200];
                int tr = 0;
                MYSQL_RES *result;
                MYSQL_ROW row;
                static int k = 0;
                printf("count = %d\n" , k++);
                rtn = sscanf(msg_str + readed + 1 , "%s %s" , client[idx].name, pwdc);
                //数据库认证登录　待写
                sprintf(sqlqwq,"SELECT * FROM userid WHERE nick='%s' and pwd = '%s';",client[idx].name, pwdc);

                char acceptd[10] = "accept";
                char denie[10] = "qwq";
                if(mysql_real_query(mysql, sqlqwq, strlen(sqlqwq))) {
                    printf("%s\n", mysql_error(mysql));
                    continue;
                }
                result = mysql_store_result(mysql);
                if( !(row  = mysql_fetch_row(result)) ) {
                        send(client[idx].fd, denie, sizeof(denie), 0);
                        mysql_free_result(result);
                        continue;
                }
                printf("%d\n",mysql_num_fields(result));
                mysql_free_result(result);
                send(client[idx].fd, acceptd, sizeof(acceptd), 0);
                sleep(1);
                whenoffline(client[idx].name, client[idx].fd);

            } else if(!strcmp(g_cmd, REG_MSG)) {
                //注册
                char regid[12],regpwd[12],sqlqwq[100];
                int correct;
                sscanf(msg_str + readed + 1, "%s %s", regid, regpwd);
                sprintf(sqlqwq ,"INSERT INTO userid (nick,pwd) VALUES ('%s','%s');" , regid, regpwd);
                printf("%s\n",sqlqwq);
                correct = mysql_real_query(mysql, sqlqwq, strlen(sqlqwq));
                if(correct) {
                    char denie[10] = "qwq";
                    send(client[idx].fd, denie, sizeof(denie), 0);
                    continue;
                }
                MYSQL_RES *result;
                char acceptd[12] = "accept";
                time_t t;
                struct tm *tblock;
                t = time( NULL);
                tblock = localtime(&t);
                char grpquery[200];
                memset(grpquery, '\0', sizeof(grpquery));
                sprintf(grpquery,"INSERT INTO syslog (timedate, log) VALUES ('%s','%s create the account.');", asctime(tblock),regid);
                if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
                    printf("%s\n",mysql_error(mysql));
                    do {
                        result = mysql_store_result( mysql);
                        mysql_free_result(result);
                    } while( !mysql_next_result( mysql));
                    continue;
                }
                do {
                    result = mysql_store_result( mysql);
                    mysql_free_result(result);
                } while( !mysql_next_result( mysql));
                send(client[idx].fd, acceptd, sizeof(acceptd), 0);
            }
        } else {
            printf("receive error msg[%d] from %s\n", len, client[idx].name);
        }
    }
    return;
}

int
judgein(int mod, char *target, char *messager)
{
    char a[30] = "SELECT * FROM ";
    char b[30] = "WHERE ";
    char mod1t[10] = "groupls";
    char mod2t[10] = "friends";
    char lastp[200];
    char mysqlquery[400];
    memset(mysqlquery, '\0', sizeof(mysqlquery));
    switch (mod) {
    case 1:
        sprintf( lastp, "memberid = '%s' and gid = '%s';", messager, target);
        sprintf( mysqlquery, "%s%s %s%s", a, mod1t, b, lastp);
        break;
    case 2:
        sprintf( lastp, "(a = '%s' and b = '%s' and status = 2) or (a = '%s' and b = '%s' and status = 2);", target, messager, messager, target);
        sprintf( mysqlquery, "%s%s %s%s", a, mod2t, b, lastp);
    }
    //printf("%s\n", mysqlquery);
    MYSQL_RES *result;
    MYSQL_ROW row;
    //tips
    if(mysql_real_query(mysql, mysqlquery, strlen(mysqlquery))) {
        printf("%s\n",mysql_error(mysql));
        printf("%s\n",mysqlquery );
        return 0;
    }
    result = mysql_store_result(mysql);
    if( !(row = mysql_fetch_row(result))) {
        mysql_free_result(result);
        return 0;
    }
    mysql_free_result(result);
    return 1;
}

int
ifonline(char *name)
{
    for(int i = 0; i < 30; i++) {
        if(client[i].fd != -1 ) {
            if(!strcmp(name, client[i].name)) {
                return client[i].fd;
            }
        }
    }
    return -1;
}

int
sendtoaim(int mod,char *recvname, int sendfd, char *string)
{
    int way = 0;
    int recvfd = 0;
    if( (recvfd = ifonline(recvname)) != -1){
        way = 1;
    }
    int i;
    if( mod == 2 || mod == 1){
        if( way == 1){
            combsend( recvfd, g_sd_str, sizeof(g_sd_str), "%s %s" , PRV_MSG, string);
        } else {
            //离线
            offline(recvname, string);
        }
    }
    if( mod == 2 || mod == 3){
        if(sendfd != -1){
            combsend( sendfd, g_sd_str, sizeof(g_sd_str), "%s %s" , PRV_MSG, string);
        }
    }
    return 0;
}

void
addfriend(char *a, char *b, int sendfd)
{
    MYSQL_RES *res;
    MYSQL_ROW  row;
    char query[200];
    int fd;
    //cas1 第一次加
    sprintf(query, "SELECT * FROM friends WHERE (a='%s' and b='%s')or(a='%s' and b='%s');", a, b, b, a);
    if(mysql_real_query(mysql, query, strlen(query))) {
        printf("%s\n", mysql_error(mysql));
        return;
    }
    res = mysql_store_result(mysql);
    if( !(row = mysql_fetch_row(res))) {
        //没有记录，添加并发送提醒。
        mysql_free_result(res);
        sprintf(query, "INSERT INTO friends VALUES ('%s','%s',0);", a, b);
        if(mysql_real_query(mysql, query, strlen(query))) {
            //回复请求发送失败
            char failed[30] = "addfriend failed, try again.";
            sendtoaim(3, b, sendfd, failed);
            return;
        }
        //回复请求已发送，发送给b;
        sendtoaim(3, b,sendfd," waiting for agreement.");
        char qwq[50];
        sprintf(qwq,"You have a friend request from %s.use /add %s to complete this request. ", a, a);
            sendtoaim(1,b,sendfd,qwq);
            return;
    }
    //有记录，是否为2，是否是同意请求。
    mysql_free_result(res);
    sprintf(query, "SELECT * FROM friends WHERE (a='%s' and b='%s' and status = 2)or(a='%s' and b='%s' and status = 2);", a, b, b, a);
    if(mysql_real_query(mysql, query, strlen(query))) {
        printf("%s\n", mysql_error(mysql));
        return ;
    }
    res = mysql_store_result(mysql);
    if( (row = mysql_fetch_row(res))) {
        //已经是好友，返回错误信息
        mysql_free_result(res);
        sendtoaim(3, b, sendfd, " You have been as friends yet" );
        return;
    }
    mysql_free_result(res);
    //不是好友，有记录。只有可能是同意
    sprintf(query, "UPDATE friends SET status=2 WHERE a='%s' and b = '%s';", b, a);
    if(mysql_real_query(mysql, query, strlen(query))) {
        printf("%s\n", mysql_error(mysql));
        return ;
    }
    char qaq[100];
    sprintf(qaq, "%s,%s now you are the friends\n", a, b);
    sendtoaim(2, b, sendfd,qaq);
    MYSQL_RES  *result;
    time_t t;
    struct tm *tblock;
    t = time( NULL);
    char grpquery[200];
    tblock = localtime(&t);
    memset(grpquery, '\0', sizeof(grpquery));
    sprintf(grpquery,"INSERT INTO syslog (timedate, log) VALUES ('%s','%s and %s become friends.');", asctime(tblock), a, b);
    if( mysql_real_query(mysql, grpquery, sizeof(grpquery))) {
        printf("%s\n",mysql_error(mysql));
        do {
            result = mysql_store_result( mysql);
            mysql_free_result(result);
        } while( !mysql_next_result( mysql));
    }
    do {
        result = mysql_store_result( mysql);
        mysql_free_result(result);
    } while( !mysql_next_result( mysql));
    return;
}

int
offline(char *recver, char *string)
{
    MYSQL_RES *result;
    MYSQL_ROW  row;
    char offquery[400];
    memset(offquery,'\0',sizeof(offquery));
    sprintf(offquery, "INSERT INTO records (recver,record) VALUES ('%s', '%s');", recver, string);
    printf("%s\n", offquery);
    if(mysql_real_query(offsql, offquery, sizeof(offquery))) {
        printf("error.");
        return 0;
    }
    //result = mysql_store_result(mysql);
    //mysql_free_result(result);
    do {
        result = mysql_store_result( offsql);
        mysql_free_result(result);
    } while( !mysql_next_result( offsql));
    return 1;
}

int
whenoffline(char *name, int fd)
{
    MYSQL_RES *result;
    MYSQL_ROW  row;
    char       msgquery[100];
    sprintf(msgquery, "SELECT record FROM records WHERE recver = '%s';", name);
    printf("%s\n",msgquery);
    if( mysql_real_query(mysql, msgquery, sizeof(msgquery))) {
        printf("%s\n", mysql_error(mysql));
        return 0;
    }
    result = mysql_store_result(mysql);
    while(row = mysql_fetch_row(result)) {
        char msgs[250];
        sprintf(msgs,"%s", row[0]);
        usleep(10000);
        sendtoaim(3, name, fd, msgs);
    }
    do {
        result = mysql_store_result( mysql);
        mysql_free_result(result);
    } while( !mysql_next_result( mysql));
    sprintf(msgquery, "DELETE FROM records WHERE recver ='%s';", name);
    if( mysql_real_query(mysql, msgquery, sizeof(msgquery))) {
        printf("%s\n", mysql_error(mysql));
        return 0;
    }
    do {
        result = mysql_store_result( mysql);
        mysql_free_result(result);
    } while( !mysql_next_result( mysql));
    result = mysql_store_result(mysql);
    return 0;

}

int
list( int mod, char *name, int fd){
    MYSQL_RES *result;
    MYSQL_ROW  row;
    char fridquery[200];
    if(mod == 1 || 2) {
        // 好友列表和在线好友列表
        memset(fridquery, '\0', sizeof(fridquery));
        sprintf(fridquery, "SELECT a FROM friends WHERE b = '%s' and status = 2;", name);
        if(mysql_real_query(mysql, fridquery, sizeof(fridquery))) {
            printf("%s" , mysql_error(mysql));
            do {
                result = mysql_store_result( mysql);
                mysql_free_result(result);
            } while( !mysql_next_result( mysql));
            return 0;
        }
        result = mysql_store_result(mysql);
        while (row  = mysql_fetch_row(result)) {
            if(mod == 2){
                if(ifonline(row[0]) != -1 ) {
                    combsend(fd, g_sd_str, sizeof(g_sd_str), "%s %s", ADDLIST, row[0]);
                    usleep(10000);
                }
                continue;
            }
            combsend(fd, g_sd_str, sizeof(g_sd_str), "%s %s", ADDLIST, row[0]);
            usleep(10000);
        }
        do {
            result = mysql_store_result( mysql);
            mysql_free_result(result);
        } while( !mysql_next_result( mysql));
        memset(fridquery, '\0', sizeof(fridquery));
        sprintf(fridquery, "SELECT b FROM friends WHERE a = '%s' and status = 2;", name);
        if(mysql_real_query(mysql, fridquery, sizeof(fridquery))) {
            printf("%s" , mysql_error(mysql));
            do {
                result = mysql_store_result( mysql);
                mysql_free_result(result);
            } while( !mysql_next_result( mysql));
            return 0;
        }
        result = mysql_store_result(mysql);
        while (row  = mysql_fetch_row(result)) {
            if(mod == 2){
                if(ifonline(row[0]) && mod == 2) {
                    combsend(fd, g_sd_str, sizeof(g_sd_str), "%s %s", ADDLIST, row[0]);
                    usleep(10000);
                    continue;
                }
            }

            combsend(fd, g_sd_str, sizeof(g_sd_str), "%s %s", ADDLIST, row[0]);
            usleep(10000);
        }
        do {
            result = mysql_store_result( mysql);
            mysql_free_result(result);
        } while( !mysql_next_result( mysql));
    }
    if( mod == 3) {
        //群成员列表
        memset(fridquery, '\0', sizeof(fridquery));
        sprintf(fridquery, "SELECT memberid FROM groupls WHERE gid = '%s';", name);
        if(mysql_real_query(mysql, fridquery, sizeof(fridquery))) {
            printf("%s" , mysql_error(mysql));
            do {
                result = mysql_store_result( mysql);
                mysql_free_result(result);
            } while( !mysql_next_result( mysql));
            return 0;
        }
        result = mysql_store_result(mysql);
        while (row  = mysql_fetch_row(result)) {
            combsend(fd, g_sd_str, sizeof(g_sd_str), "%s %s", ADDLIST, row[0]);
            usleep(10000);
        }
        do {
            result = mysql_store_result( mysql);
            mysql_free_result(result);
        } while( !mysql_next_result( mysql));
        return 0;
    }
    if( mod == 4) {
        //群列表
    }
    //三种好友，全部，在线，所在群成员
}

int
grptalk(char *grpname, char *string)
{
    MYSQL_RES *result;
    MYSQL_ROW  row;
    char a[100];
    sprintf(a, "SELECT memberid FROM groupls WHERE gid = '%s';", grpname);
    if(mysql_real_query(mysql, a, sizeof(a))) {
        printf("%s\n", mysql_errno(mysql));
        do {
            result = mysql_store_result( mysql);
            mysql_free_result(result);
        } while( !mysql_next_result( mysql));
        return 0;
    }
    result = mysql_store_result(mysql);
    while(row = mysql_fetch_row(result)) {
        char recver[20];
        strcpy(recver, row[0]);
        sendtoaim(1, recver, 0, string);
    }
    do {
        result = mysql_store_result( mysql);
        mysql_free_result(result);
    } while( !mysql_next_result( mysql));
    return 0;

}
