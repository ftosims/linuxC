#include"def.h"
#define FILENAME "client.txt"
/* include stdio.h by curses.h
 * automatically */
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
#include<fcntl.h>

//变量 登录用户信息部分
char my_id[21];
char my_pwd[21];
char cur_id[21];
char IP[20];
int  PORT;
int  svr_fd;

//信息交换部分
char recver[20];
int  recvmod;
char send_str[250];
char command [300];

//历史记录
char history[300][300] = {"\0"};
int combfw(char *str , unsigned int str_t , char *format , ... );
//curese 窗口
WINDOW *win[7],
       *winc,
       *mbox, //member
       *bbox, //usermsg box
       *tbox, //msgbox
       *ibox, //input
       *wbox, //recvername
       *obox,
       *pbox; //options

int x, y;
int new = 1;
char  ps = '$';
int mbox_t = 0;
int tbox_t = 0;
int tbox_c = 0;
int curline = 0;
int row = 0, col = 0;
//curses函数
void initial  (void);
void getmax   (void);
void redraw(int mod);
int  terminal( WINDOW *cwin, char *str, int n);
void mvwWipen(WINDOW *awin , int y , int x , int n);
int  mod;
//登录注册
int  logmod   (void);
int  login    (void);
int  zhuce    (void);
void escape   ( int);
int  cnt_host (void);


//最主要的函数，处理一切接收到的信息
void recv_msg(void);
//发送函数
ssize_t combsend(int fd , char *msg , unsigned int msg_t , char *format , ... );
int sendthefile(char *target,char *filename);
//MySQL



int
main()
{
    if(cnt_host() == -1) {
        printf("port not be used\n");
        escape(1);
    }
    logmod();
    pthread_t id;
    int precv = pthread_create(&id, NULL, (void *)recv_msg, NULL);
    if( precv) {
        printf("Create RECV_MOD failed!\n");
        exit(0);
    }
    //curese mod

    initial();
    getmaxyx(stdscr,row,col);
    bbox = newwin( 3, col-15, 0, 0);
    tbox = newwin( row-6, col-7, 3, 0);
    ibox = newwin( 3, col-15, row-3, 0);
    mbox = newwin( row, 14, 0, col-14);

    win[0] = tbox;
    win[1] = mbox;
    win[2] = bbox;
    win[3] = ibox;

    keypad(tbox, TRUE);
    keypad(ibox, TRUE);
    keypad(mbox, TRUE);
    keypad(bbox, TRUE);

    wsetscrreg(tbox, 0, 299);
    scrollok(tbox, TRUE);
    scrollok(mbox, TRUE);
    idlok(tbox, TRUE);
    leaveok(tbox, TRUE);
    winc = ibox;
    redraw(0);
    wmove(ibox,1,0);
    wrefresh(mbox);

    char string[300];
    while(1) {
        int c = 0;
        if( wmove(ibox, 1, 0) == ERR)
            printf("wmove ERR\n"),escape(0);
        wrefresh(ibox);
        if( terminal(ibox, string, 280) == ERR)
            printf("terminal ERR\n"),escape(0);
        if( string[0] != '\0') {
            if(string[0] == '/'){
                char cmd[19];
                int  num;
                sscanf( string, "%*c%s%n", cmd, &num);
                if( !strcmp( cmd, "msg")) {
                    //change recver
                    recvmod = 2;
                    char recvq[11];
                    sscanf( string, "%*s %s", recvq);
                    mod = 1;
                    strcpy(recver, recvq);
                    c = 1;
                } else if( !strcmp( cmd, "add")) {
                    //add friends
                    char recvq[11];
                    sscanf( string, "%*s %s", recvq);
                    combsend(svr_fd, send_str, sizeof(send_str), "%s %s", ADD_MSG, recvq);
                    c = 1;
                } else if( !strcmp( cmd, "join")){
                    //进入群聊
                    char recvq[11];
                    sscanf( string, "%*s %s", recvq);
                    combsend(svr_fd, send_str, sizeof(send_str), "%s %s", JOIN_MS, recvq);
                    c = 1;
                } else if( !strcmp( cmd, "del")) {
                    //删除好友
                    char recvq[11];
                    sscanf( string, "%*s %s", recvq);
                    combsend(svr_fd, send_str, sizeof(send_str), "%s %s", DEL_MSG, recvq);
                    c = 1;
                } else if( !strcmp( cmd, "chat")){
                    //群聊
                    recvmod = 1;
                    char recvq[11];
                    sscanf( string, "%*s %s", recvq);
                    mod = 1;
                    strcpy(recver, recvq);
                    c = 1;
                } else if( !strcmp( cmd, "create")) {
                    char grpname[11];
                    sscanf( string, "%*s %s", grpname);
                    combsend(svr_fd, send_str,sizeof(send_str), "%s %s", CREATEG, grpname);
                    c = 1;
                } else if( !strcmp( cmd, "getls")) {
                    char target[20];
                    char srd[20];
                    sscanf( string, "%*s %s %s", target, srd);
                    combsend(svr_fd, send_str, sizeof( send_str), "%s %s %s", CMD_MSG, target, srd);
                    c = 1;
                    mbox_t = 0;
                    werase(mbox);
                    wrefresh(mbox);
                } else if( !strcmp( cmd, "file")) {
                    char target[20];
                    char filename[400];
                    sscanf( string, "%*s %s %s", target, filename);
                    sendthefile(target,filename);
                    c = 1;
                }
            }
            if( !c){
                if( recvmod == 1) { // recvmod 群聊为1
                    //群聊发送;
                    combsend(svr_fd, send_str, sizeof(send_str), "%s %s %s", GRP_MSG, recver, string );
                } else if( recvmod == 2) {
                    //s olo or cmd 发送
                    combsend(svr_fd, send_str, sizeof(send_str), "%s %s %s", PRV_MSG, recver, string );
                }
            }
        }
        memset(string, 0, sizeof(string));
        wclear(ibox);
        redraw(mod);


    }

    return 0;

}


void
initial(void)
{
    initscr();
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr , FALSE);
    intrflush(mbox , FALSE);
    keypad(stdscr , TRUE);
    refresh();
    return;
}

//登录注册
int
logmod()
{
    printf("l to login,r to regist.");
    int a;
    while( a = getchar()) {
        if( a == 'l') {
            if(login())
                return 0;
        }
        if ( a == 'r') {
            getchar();
            printf("qwq\n");
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
    char cur_pwd[12];
    printf("Input your nick(no more than 10 letters):");
    scanf("%s%n" , my_id, &a);
    for( int i = 0; i < sizeof(my_id); i++) {
        if( my_id[i] == '\n') my_id[i] =  '\0';
    }
    strcpy( cur_id, my_id);
    printf("Input your pwd (no more than 10 letters):\033[08m");
    scanf("%s" , my_pwd );
    printf("\033[0m");
    for( int i = 0; i < sizeof(my_pwd); i++) {
        if(my_pwd[i] == '\n') my_pwd[i] = '\0';
    }
    strcpy( cur_pwd, my_pwd);
    //推去server认证。
    char pkg[40];
    sprintf(pkg, "%s %s", cur_id, cur_pwd);
    combsend(svr_fd, send_str, sizeof(send_str), "%s %s", LOG_MSG,pkg);
    len = recv(svr_fd , qwq , sizeof(qwq) , 0);
    printf("%s", qwq);
    if(len == 0) {
        printf("Connect error,please try again\n");
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
    scanf("%[a-zA-Z0-9]%n",reg_pwd, &n);
    printf("\033[0m");
    if( n == 1 || n > 10) {
        printf("Wrong format.");
        return 0;
    }
    sprintf(pkg, "%s %s", reg_id, reg_pwd);
    printf("%s", pkg);
    combsend(svr_fd, send_str, sizeof(send_str),"%s %s", REG_MSG, pkg);
    int len;
    char qwq[20];
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

void
escape(int a)
{
    //ctl-c 处理;
    endwin();
    close(svr_fd);
    if(a == 0) {
        puts("Exit chatroom");
    } else if(a == 1) {
        puts("connection break");
    } else if(a == 2) {
        puts("recvmsg break;");
    }
    exit(0);
    return;
}

void
recv_msg(void)
{
    int  len, i;
    char recv_str[400];
    char output[400] , string[300] = {'\0'};
    char mod[10]     , sender [10];
    while(1) {
        //recv the message
        memset(recv_str, '\0', sizeof(recv_str));
        len  = recv(svr_fd, recv_str, sizeof( recv_str), 0);
        if( len == 0) { //接收错误
            escape(2);
            break;
        } else if( len >= 1) {

            int readed = 0,
                ch_cnt = 0;
            sscanf(recv_str, "%s%n", mod, &readed);

            if( !strcmp(mod, CMD_MSG)) {
                //命令处理;
            } else if( !strcmp(mod, PRV_MSG)) {
                //聊天处理
                sscanf(recv_str+readed, "%[^\n]", string);
                sprintf(output, "%s", string);
            } else if( !strcmp(mod, ADDLIST)) {
                //离线消息处理。以及接收好友信息和所属群信息。
                sscanf(recv_str+readed, "%10s", string);
                sprintf(output, "%s", string);
                printf("%s\n",output);
                mvwaddstr(mbox, mbox_t, 1, output);
                mbox_t++;
                werase(ibox);
                wrefresh(ibox);
                redraw(2);
                usleep(10000);
                continue;
            } else if( !strcmp(mod, "FILE")) {
                int fd;
                char name[20];
                char data[220];
                int a;
                char *qwq = recv_str;
                sscanf(recv_str, "%*s %*s %s%n", name, &a);
                char b[2];
                sprintf(b,"%d", a);
                mvwaddstr(tbox, tbox_c, 0, b);
                tbox_c++;
                qwq = qwq +a+1;
                fd = open(name, O_WRONLY|O_CREAT|O_APPEND|O_SYNC, 0740);
                write(fd, qwq, sizeof(recv_str)-a-1);
                mvwaddstr(tbox, tbox_c, 0, qwq);
                wrefresh(tbox);
                tbox_c++;
                close(fd);
            }
            if(tbox->_curx == 0) {
                mvwaddstr(tbox, tbox_c, 0, output);
            } else {
                mvwaddstr(tbox, tbox_c, 0, output);
            }
            if( curline == 300) {
                for( i = 0; i < 300; i++) {
                    combfw(command,sizeof(command), "%s\n", history[i]);
                }
                curline = 0;
            }
            strcpy(history[curline], output);
            for( i = 0; i < (strlen(output)/(tbox->_maxx+1));i++) {
                tbox_c++;
                curline++;
                sprintf(history[curline], "^scroll up to get complete msg");
            }

            if((strlen(output)%(tbox->_maxx+1)) != 0) {
                tbox_c++;
                curline++;
                sprintf(history[curline], "^ scroll up to get complete msg");
            }
            while(tbox_c>LINES-(ibox->_maxy+1)-(bbox->_maxy+1)) {
                wscrl(tbox, 1);
                tbox_c--;
                tbox_t++;
            }

            usleep(10000);
            redraw(2);
        }
    }
    return;
}

ssize_t
combsend(int fd , char *msg , unsigned int msg_t , char *format , ... )
{
    va_list arg;
    va_start(arg , format);
    vsnprintf(msg , msg_t , format , arg);
    va_end(arg);
    return send(fd , msg , msg_t , 0);
}

void
redraw(int mod)
{


    int wlcy, wlcx;
    int i;

    wlcy = winc->_cury;
    wlcx = winc->_curx;

    for( i = 0; i <= ibox->_maxx; i++)
        mvwaddch(ibox, 0, i, '-');

    winc->_cury = wlcy;
    winc->_curx = wlcx;

    for( i = 0; i <= bbox->_maxx; i++)
        mvwaddch(bbox, 2, i, '-');
    mvwprintw(bbox, 0, 0, "Xchat");

    for( i = 0; i <= mbox->_maxy; i++)
        mvwaddch(mbox, i, 0, '|');

    if(mod == 0) {
        mvwprintw(bbox, 1, 0, "Use cmd '/msg name' chose recver(group).");
    }
    if(mod == 1) {
        mvwprintw(bbox, 1, 0, "                                        "); //清除之前显示的
        mvwprintw(bbox, 1, 0, "To %s.", recver);
    }


    for( i = 0; i < 4; i++) {
        touchwin(win[i]);
        wrefresh(win[i]);
    }
    if(mod == 2) {
        wmove(winc, winc->_cury, winc->_curx);
        wrefresh(winc);
    }
    return;
}

int
cnt_host(void)
{

    struct sockaddr_in dest0;
    /* input IP */

    printf("Input IP\n(enter = >local):");
    fgets(IP , sizeof(IP) , stdin);
    if(IP[0] == '\n') {
        sprintf(IP , "127.0.0.1");
    }
    setbuf(stdin , NULL);
    printf("will connect to %s\n" , IP);
    /* input port */
    printf("Input port(ex:8889):");
    scanf("%d" , &PORT);
    getchar();

    /* create socket */
    svr_fd = socket(PF_INET , SOCK_STREAM , 0);

    /* initialize value in dest0 */
    bzero(&dest0 , sizeof(dest0));
    dest0.sin_family = PF_INET;
    dest0.sin_port = htons(PORT);
    inet_aton(IP , &dest0.sin_addr);

    /* Connecting to server */
    if(connect(svr_fd , (struct sockaddr*)&dest0 , sizeof(dest0)) == -1) {
        puts("connect() == -1");
        return -1;
    } else {
        puts("please wait");
    }

    return 0;
}

int
terminal( WINDOW *cwin, char *str, int n)
{
    char *ostr, ec, kc;
    int c, oldx, remain;
    int prex, prey;
    int i;
    ostr = str;
    ec = erasechar();
    kc =  killchar();

    oldx = cwin->_curx;
    remain = n - 1;
    while( (c = wgetch(cwin)) != ERR
           && c != '\n' && c != '\r' ) {
        *str = c;
        touchline( cwin, cwin->_cury, 1);
        if( c == ec || c == KEY_BACKSPACE
            || c == 263 || c == 127 || c == 8) {
            *str = '\0';
            if( str != ostr) {
                if(mvwaddch(cwin, cwin->_cury, cwin->_curx - 1 , ' ') == ERR)
                    if(mvwaddch(cwin, cwin->_cury -1, cwin->_maxx, ' ' ) == ERR);
                if(wmove(cwin, cwin->_cury, cwin->_curx - 1) == ERR)
                    if(wmove(cwin, cwin->_cury - 1, cwin->_maxx ) == ERR);
                str--;
                *str = '\0';
                if( n != -1) {
                    remain++;
                }
            }
        } else if( c == kc) {
            *str = '\0';
            if(str != ostr) {
                while( str != ostr) {
                    if(mvwaddch(cwin, cwin->_cury, cwin->_curx - 1, ' ') == ERR)
                        if(mvwaddch(cwin, cwin->_cury - 1, cwin->_maxx, ' ') == ERR);
                    if(wmove(cwin, cwin->_cury, cwin->_curx - 1) == ERR)
                        if(wmove(cwin, cwin->_cury - 1, cwin->_maxx) == ERR);
                    str--;
                    if( n != -1)
                        remain++;
                }
            } else
                mvwaddch(cwin, cwin->_cury, oldx, ' ');
            wmove(cwin, cwin->_cury, oldx);
        } else if(c == KEY_UP || c == KEY_DOWN) {
            if( c == KEY_UP) {
                if(tbox_t != 0) {
                    if( strcmp(history[tbox_t] , "^ scroll up to get complete msg") == 0) {
                        mvwWipen(tbox, 0, 0, 11);
                    }
                    wscrl(tbox, -1);
                    mvwprintw(tbox, 0, 0, "%s", history[tbox_t - 1]);
                    for( i = 0; i <= 6; i++) {
                        if((i != 2) && (i != 3)) {
                            touchwin(win[i]);
                            wrefresh(win[i]);
                        }
                    }
                    tbox_t--;
                    tbox_c++;
                }
            } else if( c == KEY_DOWN) {
                if(tbox_t < (curline - (LINES - (ibox->_maxy+1)- (bbox->_maxy+1)))) {
                    wscrl(tbox, 1);
                    for( i = 0; i <= 6 ; i++) {
                        if((i != 0) && (i !=3)) {
                            touchwin(win[i]);
                            wrefresh(win[i]);
                        }
                    }
                    tbox_t++;
                    tbox_c--;
                }
            }
        } else if( c >= 32 && c<= 126) {
            mvwaddch(cwin,  cwin->_cury, cwin->_curx,c);
            wrefresh(ibox);
            if(remain) {
                str++;
                remain--;
            } else {
                mvwaddch(cwin, cwin->_cury, cwin->_curx - 1, ' ');
                wmove(cwin, cwin->_cury, cwin->_curx - 1);
            }
        }
        wrefresh(ibox);
        touchwin(wbox);
        wrefresh(wbox);
        if(wmove(cwin, cwin->_cury, cwin->_curx) == ERR)
            wmove(cwin, cwin->_cury-1, cwin->_maxx);
    }

    if( c == ERR) {
        *str = '\0';
        return (ERR);
    }
    *str = '\0';
    wrefresh(tbox);
    return (OK);
}

void
mvwWipen(WINDOW *awin , int y , int x , int n)
{
    for(int i = x ; i < x + n ; i++)
        mvwaddch(awin , y , i , ' ');
    return;
}
int
combfw(char *str , unsigned int str_t , char *format , ... )
{
    va_list arg;
    va_start(arg , format);
    vsnprintf(str , str_t , format , arg);
    va_end(arg);
    int fp;
    fp = open(my_id, O_WRONLY|O_CREAT|O_APPEND,0740);
    if(!fp) printf("%s cannot open\n" , FILENAME) , exit(1);
    write(fp , str , strlen(str));
    close(fp);
    return 0;
}

int
sendthefile(char *target,char *filename)
{
    if((access(filename, F_OK))) {
        mvwaddstr(tbox, tbox_c, 0, "File isn't exist.");
        tbox_c++;
        wrefresh(tbox);
        return 0;
    }
    char *a;
    int fd;
    fd = open(filename, O_RDONLY);
    char sendfile[60];
    char send_string[250];
    char judgestr[30];
    int len;
    a = strrchr(filename, '/');
    if( a != NULL) {
        filename = a+1;
    }
    memset(sendfile, '\0', sizeof(sendfile));
    memset(send_string, '\0', sizeof(send_string));
    while(read(fd, sendfile, sizeof(sendfile)-1)) {
        sprintf(send_string, "FILE %s %s %s", target,filename, sendfile);
        memset(sendfile, '\0', sizeof(sendfile));
        send(svr_fd, send_string, strlen(send_string), 0);
        memset(send_string, '\0', sizeof(send_string));
        wrefresh(tbox);
        sleep(3);
    }
    mvwaddstr(tbox, tbox_c, 0, "FILE COMPLETE.");
    tbox_c++;
    wrefresh(tbox);
    close(fd);
    return 1;
}
