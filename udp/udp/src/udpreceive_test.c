/* Include */
#include "udp.h"

/* Define */
#define IP_ADDRESS "239.255.0.1" // Multicast
#define PORT_NO 5555
char buf[MAX_RECEIVE_BUF_LEN];

int main_test()
{
    int rxlen;
    char receivedIpAddr[20];
    unsigned short portId;

    int sockfd = udp_ReceiveInit(IP_ADDRESS, PORT_NO);
    if (sockfd != -1)
    {
        while (1)
        {
            rxlen = udp_ReceiveData(sockfd, buf, receivedIpAddr, &portId);
            if (rxlen == -1)
            {
                printf("\n Error: udp_ReceiveData failed");
                break;
            }
            else
            {
              //  printf("\n Data received from IP Address: %s, Port: %d, Length: %d", (char*)receivedIpAddr, portId, rxlen);
            }
        }
    }
    else
    {
        printf("\n Error: udp_ReceiveInit failed");
    }

    printf("\n\n Press any key to exit..");
    getchar();
    return 0;
}