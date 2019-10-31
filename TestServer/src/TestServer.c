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
#include "CommonSocket.h"

#define BUFFERSIZE 512
#define PROTOPORT 27015
#define QLEN 6

int main(){
	char inputString[BUFFERSIZE] = "Connessione avvenuta";
	char errorString[BUFFERSIZE]="TERMINE PROCESSO CLIENT";
	int port,s,a,b;
	if(checkPort(PROTOPORT)){
		port = PROTOPORT;
	}else{
		printf("Invalid port\n");
	}

	startWSAS();

	// CREAZIONE DELLA SOCKET
	int MySocket;
	if((MySocket=createSocket())<0){
		closesocket(MySocket);
		ClearWinSock();
		return 0;
	}
	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	if((bindSocket(MySocket,"127.0.0.1",port))<0){
		closesocket(MySocket);
		ClearWinSock();
		return 0;
	}

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if((listenSocket(MySocket,QLEN))<0){
		closesocket(MySocket);
		ClearWinSock();
		return 0;
	}
	// ACCETTARE UNA NUOVA CONNESSIONE
	int clientSocket;
	char buf[BUFFERSIZE];
	char buf1[BUFFERSIZE];
	char buf2[BUFFERSIZE];
	while (1){
		printf("[SERVER]: \n");
		printf("Waiting for a client to connect...\n");

		clientSocket=acceptSocket(MySocket);
		int stringLen = strlen(inputString);
		//INVIO DATI
		invio(clientSocket,inputString,stringLen);

		ricevi(clientSocket,buf);

		ricevi(clientSocket,buf1);
		ricevi(clientSocket,buf2);
		a=atoi(buf1);
		b=atoi(buf2);

		switch(buf[0]){
			case 'a':
				s=a+b;
				memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
				sprintf(buf,"%d",s);
				stringLen = strlen(buf);
				invio(clientSocket,buf,stringLen);
				break;

			case 's':
				s=a-b;
				memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
				sprintf(buf,"%d",s);
				stringLen = strlen(buf);
				invio(clientSocket,buf,stringLen);
				break;

			case 'm':
				s=a*b;
				memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
				sprintf(buf,"%d",s);
				stringLen = strlen(buf);
				invio(clientSocket,buf,stringLen);
				break;

			case 'd':
				s=a/b;
				memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
				sprintf(buf,"%d",s);
				stringLen = strlen(buf);
				invio(clientSocket,buf,stringLen);
				break;
			default:
				stringLen = strlen(errorString);
				invio(clientSocket,errorString,stringLen);
				break;
		}
	}
}
