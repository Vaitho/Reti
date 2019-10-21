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
#define PROTOPORT 27015
#define QLEN 6

void ErrorHandler(char *errorMessage){
    printf ("%s", errorMessage);
}
void ClearWinSock(){
	#if defined WIN32
	WSACleanup();
	#endif
}

void invio(int clientSocket, char inputString[BUFFERSIZE], int stringLen);
void ricevi(int clientSocket, char buf[BUFFERSIZE]);

int main() {
	int port;
	port = PROTOPORT;

	#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (iResult != 0) {
		ErrorHandler("Error at WSAStartup()\n");
		return 0;
	}
	#endif

	// CREAZIONE DELLA SOCKET
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySocket < 0) {
		ErrorHandler("socket creation failed.\n");
		ClearWinSock();
		return -1;
	}

	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port);

	if(bind(MySocket, (struct sockaddr*) &sad, sizeof(sad))<0){
		ErrorHandler("bind() failed.\n");
		closesocket(MySocket);
		ClearWinSock();
		return -1;
	}

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if(listen (MySocket, QLEN)){
		ErrorHandler("listen() failed.\n");
		closesocket(MySocket);
		ClearWinSock();
		return -1;
	}

	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int clientSocket; // socket descriptor for the client
	int clientLen; // the size of the client address

	char buf[BUFFERSIZE];
	char buf1[BUFFERSIZE];
	printf("[SERVER]: \n");
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
		ricevi(clientSocket,buf);

		//RICEVI2
		ricevi(clientSocket, buf1);

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
				ErrorHandler("send() sent a different number of bytes than expected \n");
				closesocket(clientSocket);
				ClearWinSock();
	}
}

void ricevi(int clientSocket,char buf[BUFFERSIZE]){
	int bytesRcvd;
	int totalBytesRcvd = 0;

	printf("Received: ");
		while (totalBytesRcvd == 0){
			fflush(stdout);
			if ((bytesRcvd = recv(clientSocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
				ErrorHandler("recv() failed or connection closed prematurely \n");
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