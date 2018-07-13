#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <thread>

#define PORT 49999
#define LISTEN_MAX 1000
#define MAXLINE 1024
#define TIME_SET (200 * 1000) // ms
using namespace std;

const char *ipAddreass = "127.0.0.1";
static int check = 1;


void Server()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int client[LISTEN_MAX];
    int sockfd;
    int clientId = 0;
    int i = 0;
    int nready;
    int maxfd;
    int control = 1;
    int conn;
    int maxi;
    ssize_t n;
    char buf[MAXLINE];
    fd_set rset, allset;
    socklen_t clilen;
    time_t timeClock;
    struct timeval timeout = {0, TIME_SET};
    struct sockaddr_in serAddr,cliaddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serAddr.sin_port = htons(PORT);

    unsigned int yes = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes,
            sizeof(yes));

    if (-1 == bind(listenfd, (struct sockaddr*)&serAddr, sizeof(serAddr))) {
        printf("ser::bind failed,err=%s\n",strerror(errno));
        return ;
    }
    printf("ser::listen %d PORT\n", PORT);


    if (-1 == listen(listenfd, 10)) {
        printf("ser::listen failed\n");
        return ;
    }
    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < LISTEN_MAX; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while (control) {
        rset = allset;
        //nready = select(maxfd + 1, &rset, NULL, NULL , &timeout);
        nready = select(1023, &rset, NULL, NULL , 0);
        if (nready < 0) {
            printf("server:elect failed, err = %s\n", strerror(errno));
            control = 0;
            break;
        }
        else if (nready > 0) {
            if (FD_ISSET(listenfd, &rset)) {
               clilen = sizeof(cliaddr);
               conn = accept(listenfd, (struct sockaddr*)&cliaddr,
                      &clilen);
               for (i = 0; i < LISTEN_MAX; i++) {
                  if (client[i] < 0) {
                      client[i] = conn;
                      break;
                  }
               }
               if (i == LISTEN_MAX) {
                   printf("ser::too many clients\n");
                   return ;
               }
               printf("conn[%d] = %d\n", i, conn);
               FD_SET(conn, &allset);
               if (conn > maxfd)
                   maxfd = conn;
               if (i > maxi)
                   maxi = i;
               if(--nready < 0)
                   continue;
            }

            for (i = 0; i <= maxi; i++) {
                if ((sockfd = client[i]) < 0){
                    continue;
                }
                if (FD_ISSET(sockfd, &rset)) {
                    if ((n = recv(sockfd, buf, sizeof(buf),0)) > 0) {
                        if((strcmp(buf,"exit")) == 0) {
                            clientId++;
                            printf("ser::client %d finished\n", clientId);
                            timeClock = time(NULL);
                            close(sockfd);
                            FD_CLR(sockfd, &allset);
                            client[i] = -1;
                       }
                         else {
                             strcpy(buf, "pong\n");
                             send(sockfd, buf, sizeof(buf),0);
                         }
                     }
                     if(--nready <= 0)
                         break;
               }
            }
        }

    }
    close(listenfd);
}



void Client()
{
    int i = 0;
    int maxfd = 0;
    int maxi;
    int breakNum = LISTEN_MAX;
    int nready = 1;
    int sockfd[LISTEN_MAX];
    fd_set rset, allset;
    char buf[MAXLINE];
    struct sockaddr_in servaddr;
    time_t start[LISTEN_MAX];
    struct timeval timeout = {0, TIME_SET};
    time_t startTime,endTime;
    startTime = time(NULL);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(ipAddreass);
    memset(&start,0,sizeof(start));
    inet_pton(AF_INET, ipAddreass, &servaddr.sin_addr);
    FD_ZERO(&allset);

    while (i < LISTEN_MAX) {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd[i] == -1) {
            printf("client:create socket failed. err = %s\n", strerror(errno));
            FD_SET(sockfd[i], &allset);
            i++;
            continue;
        }
        if (connect(sockfd[i], (struct sockaddr*)&servaddr,
                    sizeof(servaddr)) < 0) {
            printf("connect %d failed, err = %s\n",i, strerror(errno));
            sockfd[i] = -1;
            FD_SET(sockfd[i], &allset);
            i++;
            continue;
        }

        printf("sockfd[%d] = %d\n", i, sockfd[i]);
        strcpy(buf, "ping\n");
        send(sockfd[i], buf, strlen(buf)+1, 0);
        maxfd = (maxfd > sockfd[i] ? maxfd : sockfd[i]) + 1;
        FD_SET(sockfd[i], &allset);
        i++;
    }
    FD_ZERO(&rset);
    maxi = -1;

    while (breakNum) {
        rset = allset;
        nready = select(maxfd, &rset, NULL, NULL, &timeout);
        if (nready < 0) {
           printf("cli::select false. err=%s \n",strerror(errno));
           return ;
        }else if(nready >= 0) {
            for (i = 0; i < LISTEN_MAX; i++) {
                if (sockfd[i] < 0) {
                    breakNum--;
                    continue;
                }
                if ((start[i] > 0) && ((time(NULL)-start[i])>=10)) {
                    breakNum--;
                    send(sockfd[i], "exit", 5, 0);
                    close(sockfd[i]);
                    FD_CLR(sockfd[i], &allset);
                    sockfd[i] = -1;
                    start[i] = 0;
                    printf("cli::now breakNum:%d exit\n", breakNum);
                }
                if (FD_ISSET(sockfd[i], &rset)) {
                    if(recv(sockfd[i], buf, sizeof(buf), 0) > 0) {
                        start[i] = time(NULL);
                        printf("i = %d cli::receive server:%s", i, buf);
                    }
                    else {
                        printf("cli::server send failed\n");
                        close(sockfd[i]);
                        FD_CLR(sockfd[i], &allset);
                        sockfd[i] = -1;
                    }
                }
            }
        }
    }
    endTime = time(NULL);
    printf("%d client finished,time:%ld\n", LISTEN_MAX-breakNum,
            endTime - startTime);
    check = 0;
}



int main()
{
    thread t1(Server);
    sleep(2);
    thread t2(Client);

    t1.join();
    t2.join();
    return 0;
}
