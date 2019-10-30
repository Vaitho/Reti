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
	int Csocket;
	if((Csocket=createSocket())<0){
		ErrorHandler("socket creation failed.\n");
		return 0;
	}
	//CONNESSIONE
	if((connectClient(Csocket,"127.0.0.1",port))<0){
			closesocket(Csocket);
			ClearWinSock();
	}
	//RICEVI
	char buf[BUFFERSIZE]; // buffer for data from the server
	ricevi(Csocket,buf);
	printf("Seleziona l'operazione da fare(inserendo a/s/m/d):" );
	scanf("%s",buf);
	int stringLen = strlen(buf);
	invio(Csocket,buf,stringLen);

	char a[BUFFERSIZE];
	char b[BUFFERSIZE];

	printf("Inserisci operando:");
	scanf("%s",a);
	stringLen = strlen(a);
	invio(Csocket,a,stringLen);


	printf("Inserisci secondo operando:");
	scanf("%s",b);
	stringLen = strlen(b);
	invio(Csocket,b,stringLen);
	memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));
	ricevi(Csocket,buf);
	if(buf[0]="T"){
		printf("chiusura connessione");
		printf("\n");
	}else{
		printf("Il risultato dell'operazione e':");
		printf("%s", buf);
		printf("\n");
	}
	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	ClearWinSock();
	system("PAUSE");
	return(0);
}
