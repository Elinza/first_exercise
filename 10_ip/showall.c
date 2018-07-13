#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE 1024



int main(int argc, char *argv[])
{
    struct ifreq *ifr = NULL;
    struct ifconf ifc;
    unsigned char buf[BUFF_SIZE] = {0};
    int sock = socket(AF_INET, SOCK_DGRAM,0);
    char temp[BUFF_SIZE];
    ifc.ifc_len = BUFF_SIZE;
    ifc.ifc_buf = (caddr_t)buf;

    while (argv[1] == NULL) {
        printf("Please enter a interface:");
        scanf("%s", temp);
        argv[1] = (char *)malloc(strlen(temp));
        strcpy(argv[1], temp);
    }

    if (sock < 0) {
        perror("create socket failed:");
        return -1;
    }

    if (ioctl(sock, SIOCGIFCONF, (char *)&ifc) < 0) {
        perror("ioctl-conf:");
        close(sock);
        return -1;
    }

    ifr = (struct ifreq*)buf;
    strcpy(ifr->ifr_name, argv[1]);
    free(argv[1]);
        printf("\n%s", ifr->ifr_name);

        if (ioctl(sock, SIOCGIFFLAGS, ifr) >= 0) {
            if (IFF_UP & ifr->ifr_flags)
                printf(" [UP]\n");
            else
                printf("[DOWN]\n");
        }
        else
            perror("ioctl SIOCGIFFLAGS error\n");

        if (ioctl(sock, SIOCGIFHWADDR, ifr) >= 0) {
            u_int8_t hd[6] = {0};
            memcpy(hd, ifr->ifr_hwaddr.sa_data, sizeof(hd));
            printf("MACaddr:%02x%02x%02x%02x%02x%02x\n", hd[0], hd[1], hd[2],
                    hd[3], hd[4], hd[5]);
        }
        else
            printf("ioctl SIOCGIFHWADDR error\n");

        if (ioctl(sock, SIOCGIFADDR, ifr) >= 0)
            printf("IP:%s\n",inet_ntoa(((struct sockaddr_in *)
                            &(ifr->ifr_addr))->sin_addr));
        else
            perror("ioctl SIOCGIFADDR error\n");

        if (ioctl(sock, SIOCGIFBRDADDR, ifr) >=0)
            printf("Bcast:%s\n", inet_ntoa(((struct sockaddr_in *)
                            &(ifr->ifr_broadaddr))->sin_addr));
        else
            perror("ioctl SIOCGIFBRDADDR error\n");

        if (ioctl(sock, SIOCGIFNETMASK, ifr) >= 0)
            printf("Mask:%s\n", inet_ntoa(((struct sockaddr_in *)
                            &(ifr->ifr_netmask))->sin_addr));
        else
            perror("ioctl SIOCGIFNETMASK error\n");

    close(sock);
    return 0;
}
