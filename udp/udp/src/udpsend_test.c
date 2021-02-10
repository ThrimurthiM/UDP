/* Include */
#include "udp.h"

/* Define */
#define IP_ADDRESS "239.255.0.1" // Multicast
#define PORT_NO 5555
#define WRITE_LENGTH    15000

int main()
{
    char data[WRITE_LENGTH];
    memset(data, 0xaa, WRITE_LENGTH);

    int sockfd = udp_SendInit(IP_ADDRESS, PORT_NO);
    if (sockfd != -1)
    {
        while (1)
        {
            static unsigned int i = 0;
            data[0] = i++;
            // Sending 1500 bytes of dummy data
            int len = udp_SendData(sockfd, data, WRITE_LENGTH);
            if (len == -1)
            {
                printf("\n Error: udp_SendData failed");
                break;
            }
            else
            {
                printf("\n Data sent to IP Address: %s:%d, Length: %d", (char*)IP_ADDRESS, PORT_NO, WRITE_LENGTH);
            }
#ifdef _WIN32
            Sleep(1000);
#else
            sleep(1000)
#endif

        }
    }
    else
    {
        printf("\n Error: udp_SendInit failed");
    }
    printf("\n\n Press any key to exit..");
    getchar();
    return 0;
}