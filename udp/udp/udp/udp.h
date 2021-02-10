#ifndef _UDP_H_
#define _UDP_H_

#ifdef __cplusplus
extern "C" {
#endif

    /* Include */
#ifdef _WIN32
#include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
#include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
#include <Windows.h>
#elif linux
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#define MAX_RECEIVE_BUF_LEN (65536 * 10)  //Max length of buffer

/* External Interfaces */
    int udp_SendInit(char* ipAddr, unsigned short port);
    int udp_ReceiveInit(char* ipAddr, unsigned short port);
    int udp_SendData(int sockfd, char* data, int len);
    int udp_ReceiveData(int sockfd, char* data, char* receivedIpAddr, unsigned short* receivedPortId);

#ifdef __cplusplus
}; //extern "C"
#endif
#endif
