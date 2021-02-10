/* Includes */
#include "udp.h"

/* Define */
#define IP_PROTOCOL 0 

static int chunkSize = 1316;
char residuebUffer[1316];
int flag = 0;
long validBufferLength = 0;
struct sockaddr_in send_addr, receive_addr_me, receive_addr_other;
struct ip_mreq mreq;
int addrlen = sizeof(send_addr);

int udp_SendInit(char* ipAddr, unsigned short port)
{
    int sockfd = -1;

#ifdef _WIN32
    //
    // Initialize Windows Socket API with given VERSION.
    //
    WSADATA wsaData;
    if (WSAStartup(0x0101, &wsaData)) {
        printf("\n UDP WSAStartup failed");
        return -1;
    }
#endif

    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(port);
    send_addr.sin_addr.s_addr = inet_addr(ipAddr);
    // socket()
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL)) == -1)
    {
        printf("\n UDP socket create failed");
        return -1;
    }

    return sockfd;
}

int udp_ReceiveInit(char* ipAddr, unsigned short port)
{
    int sockfd = -1;
#ifdef _WIN32
    //
    // Initialize Windows Socket API with given VERSION.
    //
    WSADATA wsaData;
    if (WSAStartup(0x0101, &wsaData)) {
        printf("\n UDP WSAStartup failed");
        return -1;
    }
#endif

    //create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("\n UDP socket create failed");
        return -1;
    }

    // zero out the structure
    memset((char *)&receive_addr_me, 0, sizeof(receive_addr_me));

    receive_addr_me.sin_family = AF_INET;
    receive_addr_me.sin_port = htons(port);
    receive_addr_me.sin_addr.s_addr = htonl(INADDR_ANY);
    //receive_addr_me.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    //bind socket to port
    if (bind(sockfd, (struct sockaddr*)&receive_addr_me, sizeof(receive_addr_me)) == -1)
    {
        printf("\n UDP socket bind failed");
        return -1;
    }

    mreq.imr_multiaddr.s_addr = inet_addr((const char FAR *)ipAddr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        printf("\n UDP setsockopt mreq failed");
        return -1;
    }

    fflush(stdout);

    return sockfd;
}

int udp_SendData(int sockfd, char* data, int len)
{
    int retVal = -1;
    int dataRemain = len;
    int fromIndex = 0;

    if (flag == 1)
    {
        int dataCopySize = (int)(chunkSize - validBufferLength);
        memcpy(residuebUffer + validBufferLength, data, sizeof(char) * dataCopySize);

        retVal = sendto(sockfd, residuebUffer, chunkSize, 0, (struct sockaddr*)&send_addr, addrlen);
        if (retVal == -1)
        {
            printf("\n Send Error : %s ", strerror(errno));
        }
        fromIndex += dataCopySize;
        dataRemain -= dataCopySize;
    }

    while (dataRemain >= chunkSize)
    {
        retVal = sendto(sockfd, data + fromIndex, chunkSize, 0, (struct sockaddr*)&send_addr, addrlen);
        if (retVal == -1)
        {
            printf("\n Send Error : %s ", strerror(errno));
            return -1;
        }
        fromIndex += (int)chunkSize;
        dataRemain -= chunkSize;
    }

    if (dataRemain > 0)
    {
        memcpy(residuebUffer, data + fromIndex, sizeof(char) * dataRemain);
        validBufferLength = dataRemain;
        flag = 1;
    }
    else
    {
        validBufferLength = 0;
        flag = 0;
    }
    return retVal;
}

int udp_ReceiveData(int sockfd, char* data, char* receivedIpAddr, unsigned short* receivedPortId)
{
    int nbytes = recvfrom(sockfd, data, MAX_RECEIVE_BUF_LEN, 0, (struct sockaddr*)&receive_addr_other, &addrlen);
    if (nbytes < 0)
    {
        printf("\n Receive Error");
        return -1;
    }
    printf("\n Packet of length %d received from %s:%d", nbytes, inet_ntoa(receive_addr_other.sin_addr), ntohs(receive_addr_other.sin_port));
   /* *receivedIpAddr = (char*)inet_ntoa(receive_addr_other.sin_addr);
    *receivedPortId = (unsigned short)ntohs(receive_addr_other.sin_port);*/
    return nbytes;
}



