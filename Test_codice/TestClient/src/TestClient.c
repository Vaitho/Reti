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
#include "CommonSocket.h"

#define BUFFERSIZE 512
#define PROTOPORT 27015 // Numero di porta di default

int main(void) {
	int port;
	if(checkPort(PROTOPORT)){
		port = PROTOPORT;
		printf("Port okay!\n");
	}else{
		printf("Invalid port\n");
	}

	startWSAS();
	//CREAZIONE SOCKET
	int Csocket =createSocket(Csocket);
	//CONNESSIONE
	connectClient(Csocket,"127.0.0.1",port);
	//RICEVI
	char buf[BUFFERSIZE]; // buffer for data from the server
	ricevi(Csocket,buf);
	memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));

	char stringa1[BUFFERSIZE];
	char stringa2[BUFFERSIZE];

	//INSERIMENTO PRIMA STRINGA
	printf("Inserisci le due stringhe da inviare al server: ");
	fflush(stdout);
	scanf("%s", stringa1);
	int stringLen = strlen(stringa1);
	printf("\n");
	fflush(stdout);

	//INSERIMENTO SECONDA STRINGA
	printf("Inserisci la seconda stringa: ");
	fflush(stdout);
	scanf("%s", stringa2);
	int stringLen2 = strlen(stringa2);
	printf("\n");
	fflush(stdout);

	//PRIMA STRINGA INVIATA
	invio(Csocket,stringa1,stringLen);

	//SECONDA STRINGA INVIATA
	invio(Csocket,stringa2,stringLen2);
	//RICEVI LE STRINGHE MODIFICATE
	memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
	ricevi(Csocket, buf);
	memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
	ricevi(Csocket, buf);

	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	ClearWinSock();
	system("PAUSE");
	return(0);
}
