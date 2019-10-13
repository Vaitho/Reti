#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define BUFFERSIZE 512
#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // size of request queue

void ErrorHandler(char *errorMessage){
    printf ("%s", errorMessage);
}
void ClearWinSock(){
    WSACleanup();

}

void invio(int clientSocket,char inputString[BUFFERSIZE],int stringLen);
void ricevi(int totalBytesRcvd,int bytesRcvd,int clientSocket,char buf[BUFFERSIZE]);


int main() {
	int port;
	port = PROTOPORT; // use default port number

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return 0;
	}

	// CREAZIONE DELLA SOCKET
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port);


	bind(MySocket, (struct sockaddr*) &sad, sizeof(sad));

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	listen (MySocket, QLEN);

	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int clientSocket; // socket descriptor for the client
	int clientLen; // the size of the client address

	//VARIABILI RICEVI
	int bytesRcvd;
	int totalBytesRcvd = 0;

	char buf[BUFFERSIZE];
	char buf1[BUFFERSIZE];

	printf("Waiting for a client to connect...\n");
	fflush(stdout);
	while (1){
		fflush(stdout);
		clientLen = sizeof(cad); // set the size of the client address
		if ((clientSocket = accept(MySocket, (struct sockaddr *)&cad,&clientLen)) < 0) {
			ErrorHandler("accept() failed.\n");
			// CHIUSURA DELLA CONNESSIONE
			closesocket(MySocket);
			ClearWinSock();
			return 0;
		}
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
		char inputString[BUFFERSIZE] = "Connessione avvenuta";
		int stringLen = strlen(inputString);

		//INVIO DATI
		invio(clientSocket,inputString,stringLen);

		//RICEVI
		ricevi(totalBytesRcvd,bytesRcvd,clientSocket,buf);

		totalBytesRcvd=0;

		//RICEVI2
		ricevi(totalBytesRcvd, bytesRcvd, clientSocket, buf1);

		totalBytesRcvd=0;

		  strupr(buf);
		  //INVIO 1
		  invio(clientSocket,buf,stringLen);

		  strlwr(buf1);
		  //INVIO 2
		  invio(clientSocket,buf1,stringLen);
	   }
}

void invio(int clientSocket,char inputString[BUFFERSIZE],int stringLen){
	if (send(clientSocket, inputString, stringLen, 0) != stringLen) {
				ErrorHandler("send() sent a different number of bytes than expected");
				closesocket(clientSocket);
				ClearWinSock();
	}
}

void ricevi(int totalBytesRcvd,int bytesRcvd,int clientSocket,char buf[BUFFERSIZE]){
	printf("Received: ");
		while (totalBytesRcvd == 0){
			fflush(stdout);
			if ((bytesRcvd = recv(clientSocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
				ErrorHandler("recv() failed or connection closed prematurely");
				closesocket(clientSocket);
				ClearWinSock();
			}
			totalBytesRcvd += bytesRcvd;
			buf[bytesRcvd] = '\0';
			printf("%s", buf);
			fflush(stdout);
			printf("\n");
		}
}
