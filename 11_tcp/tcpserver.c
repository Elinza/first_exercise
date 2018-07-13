#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>

#define PORT 8888
#define QUEUE 20
#define BUF_SIZE 1024



int main()
{
    int sockSer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockSer, (struct sockaddr *)&serAddr,
            sizeof(serAddr)) == -1) {
        printf("bind, err = %s\n", strerror(errno));
        return 0;
    }

    printf("listen %d port\n",PORT);

    if (listen(sockSer, QUEUE) == -1) {
        printf("listen, err = %s\n", strerror(errno));
        return 0;
    }

    char buf[BUF_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t length = sizeof(clientAddr);

    printf("wait for client\n");

    int conn;

    while (1) {
        conn =  accept(sockSer, (struct sockaddr*)&clientAddr, &length);
        if(conn == -1) {
           continue;
        }
        while (1) {
          memset(buf, 0, sizeof(buf));
          int len = recv(conn, buf, sizeof(buf), 0);
          printf("message from client:%s and reply:", buf);
          if (strcmp(buf, "exit\n") == 0 || len <= 0) {
              printf("client quit!!!\n");
              break;
          }
          memset(buf, 0, sizeof(buf));
          fgets(buf, sizeof(buf), stdin);
          send(conn, buf, (strlen(buf) + 1), 0);
          printf("send message to client:%s\n", buf);
        }
        close(conn);
    }

    close(sockSer);

    return 0;
}
