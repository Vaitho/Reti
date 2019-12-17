#ifndef _COMMONSOCKET_H
#define _COMMONSOCKET_H

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define BUFFERSIZE 512
#define LOCALHOST "127.0.0.1"
#define DEFAULTPORT 27015

//ESONERO 2
char* getHostIp(char* hostname);
char* getHostName(char* ip_address);
struct sockaddr_in setAddress(short sin_family,char* ip_address,int port);

int checkPort(int port);
void startWSAS();
void ClearWinSock();
void ErrorHandler(char *errorMessage);
int createSocket();
int bindSocket(int MySocket, char* ip, int port);
int listenSocket(int MySocket, int len);
int acceptSocket(int MySocket);
void invio(int clientSocket,char inputString[BUFFERSIZE],int stringLen);
void ricevi(int clientSocket,char buf[BUFFERSIZE]);
int connectClient(int socket, char* ip, int port);
//ESONERO 1
int receivingInt(int* socket, int* i);
int sendingInt(int* socket, int i);
#endif
