#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
    pid_t pid;
    pid = fork();
    switch (pid){
    case 0:
        while (1){
            printf("a bac pro ,PID %d parent id %d\n",getpid(), getppid());
            sleep(3);
        }
    case (-1):
        perror("proc creat f\n");
    default:
        printf("im par my id is %d\n", getppid());
        sleep(5);
        exit(0);

    }
    return 0;
}
