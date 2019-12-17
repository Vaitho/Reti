/*
 ============================================================================
 Name        : ServerE.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

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
#include <string.h>

#include "CommonSocket.h"

#define BUFFERSIZE 256
#define ADDRESS "127.0.0.1"
#define PORT 9002
#define QUEUELENGTH 5

void handleConnections(int server_socket);

int main(void) {
#if defined WIN32 // initialize Winsock
	startWSAS();
#endif
	int server_socket;
	struct sockaddr_in server_address;
	//Creo il socket del server
	server_socket = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);//tipo socket(famiglia protocolli usati),tipo connessione,protocollo
	if(server_socket<0){
		printf("Errore durante la creazione del socket\n");
		system("pause");
		return ERROR;
	}
	//Specifico protocollo,indirizzo ip e porta del server
	server_address=setAddress(AF_INET,ADDRESS,PORT);

	//Leggo il socket specificato con l'ip e la porta identificati da server_address
	if(bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address))<0){//Socket,casting della struttura,grandezza della struttura calcolata da sizeof
		printf("Errore durante il bindind\n");
		system("pause");
		return ERROR;
	}

	//Metodo che descrive il funzionamento vero e proprio del server
	handleConnections(server_socket);

	//Chiudo il socket
	closesocket(server_socket);
	ClearWinSock();
	system("pause");
	return EXIT_SUCCESS;
	system("Pause");
	return EXIT_SUCCESS;
}


void handleConnections(int server_socket){
	char messaggio[BUFFERSIZE];
	char c;
	int client_socket;
	char* clientName;
	struct sockaddr_in client_address;
	int clientLength = sizeof(client_address);
	while(1){
		printf("In attesa di messaggio dal client...\n");
		recvfrom(server_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr*)&client_address, &clientLength);
		clientName=getHostName(inet_ntoa(client_address.sin_addr));
		printf("Messaggio ricevuto dal client con nome: %s\n",clientName);
		printf("Messaggio: %s\n", messaggio);
		strcpy(messaggio,"OK");
		sendto(server_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr *)&client_address,sizeof(messaggio));
		recvfrom(server_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr*)&client_address, &clientLength);
		while(messaggio[1]!='1'){
			printf("%c\n",messaggio[0]);
			messaggio[0]=toupper(messaggio[0]);
			sendto(server_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr *)&client_address,sizeof(messaggio));
			recvfrom(server_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr*)&client_address, &clientLength);
		}
	}
}
