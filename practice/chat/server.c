#include"setting.h"
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




int sockfd , clientfd , svr_port , ch , online = 0 , curline=0;

int
main(void)
{
    int i ,len;
    //创建相关的套接字-
    struct sockaddr_in chatroom_addr , client_addr;
    printf("Input port(ex:21105):") , scanf("%d" , &svr_port);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if( !(~sockfd))
        puts("Create socket failed"),exit(1);

    bzero(&chatroom_addr, sizeof(chatroom_addr));
    chatroom_addr.sin_family = PF_INET;
    chatroom_addr.sin_port = htons(svr_port);
    chatroom_addr.sin_addr.s_addr = INADDR_ANY;

    //bind()
    if( !(~bind(sockfd, (struct sockaddr*)&chatroom_addr, sizeof(chatroom_addr))))
        puts("bind addr failed"), exit(1);
    //listen

}

