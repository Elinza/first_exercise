#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>

#define PORT 8888
#define SIZE 1024



char* SERVER_IP = "127.0.0.1";

int main()
{
    int sockCli = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = inet_addr(SERVER_IP); 

    printf("link %s:%d\n", SERVER_IP, PORT);

    if (connect(sockCli, (struct sockaddr*)&serAddr, sizeof(serAddr)) < 0) {
        printf("connect\n");
        return 0;
    }

    printf("link successfully\n");

    char sendBuf[SIZE];
    char recvBuf[SIZE];

    while (fgets(sendBuf, sizeof(sendBuf), stdin) != NULL) {
        printf("send message to server:%s", sendBuf);
        send(sockCli, sendBuf, (strlen(sendBuf)+1), 0);
        if (strcmp(sendBuf, "exit\n") == 0)
            break;
        recv(sockCli, recvBuf, sizeof(recvBuf), 0);
        printf("receive from server:%s\n\nand send:", recvBuf);

        memset(sendBuf, 0, sizeof(sendBuf));
        memset(recvBuf, 0, sizeof(recvBuf));
    }

    close(sockCli);

    return 0;
}
