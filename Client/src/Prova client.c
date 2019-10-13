#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 512
#define PROTOPORT 27015 // Numero di porta di default

void ErrorHandler(char *errorMessage){
    printf("%s",errorMessage);
}
void ClearWinSock(){
    WSACleanup();
}

int main(void) {
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
		return -1;
	}
	// CREAZIONE DELLA SOCKET
	int Csocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// COSTRUZIONE DELL INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(PROTOPORT); // Server port
	// CONNESSIONE AL SERVER
	connect(Csocket, (struct sockaddr *)&sad, sizeof(sad));

	//RICEVI
	int bytesRcvd;
	int totalBytesRcvd = 0;
	char buf[BUFFERSIZE]; // buffer for data from the server

	printf("Received: "); // Setup to print the echoed string
	while (totalBytesRcvd == 0) {
		fflush(stdout);
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
		buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
		printf("%s", buf); // Print the echo buffer
		fflush(stdout);
		printf("\n");
		fflush(stdout);
	}

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
	if (send(Csocket, stringa1, stringLen, 0) != stringLen) {
			ErrorHandler("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
	}

	//SECONDA STRINGA INVIATA
	if (send(Csocket, stringa2, stringLen2, 0) != stringLen2) {
			ErrorHandler("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
	}

	//RICEVI LE STRINGHE MODIFICATE

	totalBytesRcvd=0;
	memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));

	while (totalBytesRcvd == 0) {
		fflush(stdout);
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
		buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
		printf("%s", buf); // Print the echo buffer
		fflush(stdout);
		printf("\n");
		fflush(stdout);
	}

	totalBytesRcvd=0;
	memset(buf, 0, BUFFERSIZE*sizeof(buf[0]));

	while (totalBytesRcvd == 0) {
		fflush(stdout);
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
		buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
		printf("%s", buf); // Print the echo buffer
		fflush(stdout);
		printf("\n");
		fflush(stdout);
	}

	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	ClearWinSock();
	system("PAUSE");
	return(0);
}
