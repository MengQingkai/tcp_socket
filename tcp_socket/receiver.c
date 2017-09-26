#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

//#define BUF_SIZE 100000000
#define BUF_SIZE 144800000
#define MAXNUM 100
#define LISTEN_NO 10
#define THREADNUM 1
//#define BASIC_PORT 6000
#define err_exit(msg) (perror(msg),(exit(EXIT_FAILURE)))

void *rcv_msg(void *fd)
{
   // char rcvbuf[BUF_SIZE];
    char *rcvbuf;
    rcvbuf=malloc(sizeof(char)*BUF_SIZE);
    int sockfd =*(int *)fd;
    int connfd;
    listen(sockfd, LISTEN_NO);
    connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);
    int count=0;
    while(1)
    {
        recv(connfd, rcvbuf, BUF_SIZE*sizeof(rcvbuf[0]), 0);
        //printf("%d\n",count++);
    }

}

int main(int argc, char *argv[])
{
    /*
     * please input the parameter as ./receiver port
    */
    int i;
    int sockfd[MAXNUM],bindfd[MAXNUM];
    struct sockaddr_in clientAddr[MAXNUM];
    int port;
    int threadnum;
    pthread_t rcv_tid;
    port=atoi(argv[1]);
//
//
    printf("normal\n");
//
//
    for(i=0; i<THREADNUM; i++)
    {
        sockfd[i]=socket(PF_INET, SOCK_STREAM, 0); //socket
        if(sockfd[i]<0)
        {
            err_exit(">>socket");
        }
        memset(&clientAddr[i], 0, sizeof(clientAddr[i]));
        clientAddr[i].sin_family = AF_INET;
        clientAddr[i].sin_port = htons(port+i);
        clientAddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        bindfd[i]=bind(sockfd[i],(struct sockaddr*)&clientAddr[i],sizeof(clientAddr[i])); //bind
        if(bindfd[i]<0)
        {
            err_exit(">>bind");
        }
        listen(sockfd[i],LISTEN_NO);
    }
    for(i=0; i<THREADNUM; i++)
    {
        printf("%d\n",i);
        if(pthread_create(&rcv_tid, NULL, rcv_msg, &sockfd[i])==-1)
        {
            err_exit("pthread_create");
        }
    }
    int sleepnum = 3*(100000-(port % 1000));
    sleep(sleepnum);
    return 0;
}
