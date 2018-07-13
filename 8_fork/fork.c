#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define TEXT_LEN 1024
#define MSG_TYPE 3

struct Msg_s
{
    long int msgType;
    char msgText[TEXT_LEN];
};

int main ()
{
    pid_t pid;
    int childrun = 1;
    int fatherrun = 1;
    struct Msg_s message;
    int msgId;
    char buf[TEXT_LEN];

    msgId = msgget((key_t)1236,0666|IPC_CREAT);
    if ((pid = fork()) < 0) {
        printf("fork failed\n");
    }
    else if (pid == 0) {
        while (childrun) {
            message.msgType = MSG_TYPE;
            sleep(1);
            printf("enter message:");
            fgets(buf, TEXT_LEN, stdin);

            strcpy(message.msgText, buf);
           if (msgsnd(msgId, (void *)&message, sizeof(struct Msg_s), 0)
                   != -1 )
               printf("child success!I send %s\n",message.msgText);
           if (strncmp(buf, "bye", 3) == 0){
               childrun = 0;
           }
        }
    }
    else if (pid > 0) {
        while (fatherrun) {
            if((msgrcv(msgId, &message, sizeof(struct Msg_s), MSG_TYPE, 0))
                    != -1)
                printf("father success!I get %s\n", message.msgText);
            if(strncmp(message.msgText, "bye", 3) == 0)
                fatherrun = 0;


        }
    }

    return 0;
}
