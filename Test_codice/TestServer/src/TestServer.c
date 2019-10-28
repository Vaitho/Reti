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
	int port;
	if(checkPort(PROTOPORT)){
		port = PROTOPORT;
		printf("Port okay!\n");
	}else{
		printf("Invalid port\n");
	}

	startWSAS();

	// CREAZIONE DELLA SOCKET
	int MySocket=createSocket(MySocket);

	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	bindSocket(MySocket,"127.0.0.1",port);

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	listenSocket(MySocket,QLEN);

	// ACCETTARE UNA NUOVA CONNESSIONE
	char buf[BUFFERSIZE];
	char buf1[BUFFERSIZE];
	int clientSocket;
	while (1){
		printf("[SERVER]: \n");
		printf("Waiting for a client to connect...\n");

		clientSocket=acceptSocket(MySocket);

		char inputString[BUFFERSIZE] = "Connessione avvenuta";
		int stringLen = strlen(inputString);

		//INVIO DATI
		invio(clientSocket,inputString,stringLen);
		printf("[CLIENT]: \n");
		//RICEVI
		ricevi(clientSocket,buf);
		//RICEVI2
		ricevi(clientSocket,buf1);


		  strupr(buf);
		  //INVIO 1
		  invio(clientSocket,buf,stringLen);

		  strlwr(buf1);
		  //INVIO 2
		  invio(clientSocket,buf1,stringLen);
	   }
}
