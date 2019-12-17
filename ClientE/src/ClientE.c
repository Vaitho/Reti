/*
 ============================================================================
 Name        : ClientE.c
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

#define BUFFERSIZE 256

#include "CommonSocket.h"

void handleConnection(struct sockaddr_in server_address,int client_socket);

int main(void) {
#if defined WIN32
	startWSAS();
#endif
	char * ip;
	char host[BUFFERSIZE];
	int port;
	int client_socket;
	struct sockaddr_in server_address;

	printf("Inserisci nome host:");
	scanf("%s",host);
	printf("Inserisci numero di porta:");
	scanf("%d",&port);
	ip=getHostIp(host);

	//creo il socket
	client_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //tipo socket(famiglia protocolli usati),tipo connessione,protocollo
	if(socket<0){
		printf("Errore durante la creazione del socket\n");
		system("pause");
		return ERROR;
	}
	//specifico l'indirizzo e la porta del socket
	server_address = setAddress(AF_INET,ip,port);

	handleConnection(server_address,client_socket);

	system("pause");
	return EXIT_SUCCESS;
}


void handleConnection(struct sockaddr_in server_address,int client_socket){
	//RICEZIONE DATI DAL SERVER
	char c[BUFFERSIZE];
	int i=0;
	char stringa[BUFFERSIZE];
	int serverLength=sizeof(server_address);
	char messaggio[BUFFERSIZE]="Messaggio iniziale client";
	sendto(client_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr*)&server_address, sizeof(messaggio));
	recvfrom(client_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr*)&server_address, &serverLength);
	printf("%s\n",messaggio);
	printf("Inserisci una stringa di caratteri:");
	scanf("%s",stringa);
	while(stringa[i]){
		c[0]=stringa[i];
		c[0]=tolower(c[0]);
		c[1]='0';
		if(c[0]=='a' || c[0]=='e' || c[0]=='i' || c[0]=='o' || c[0]=='u'){
			sendto(client_socket, c, BUFFERSIZE, 0, (struct sockaddr*)&server_address, sizeof(c));
			recvfrom(client_socket, messaggio, BUFFERSIZE, 0, (struct sockaddr*)&server_address, &serverLength);
			printf("Vocale ricevuta dal server:%c\n",messaggio[0]);
		}
		i++;
	}
	c[1]='1';
	sendto(client_socket, c, BUFFERSIZE, 0, (struct sockaddr*)&server_address, sizeof(c));
}
