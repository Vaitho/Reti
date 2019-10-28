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
#define BUFFERSIZE 512
#define PROTOPORT 27015 // Numero di porta di default

void ErrorHandler(char *errorMessage){
    printf("%s",errorMessage);
}
void ClearWinSock(){
    WSACleanup();
}

void invio(int clientSocket, char inputString[BUFFERSIZE],int stringLen);
void ricevi(int clientSocket, char buf[BUFFERSIZE]);

int main(void) {
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
		return -1;
	}
	// CREAZIONE DELLA SOCKET
	int Csocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Csocket < 0) {
		ErrorHandler("socket creation failed.\n");
		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}

	// COSTRUZIONE DELL INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(PROTOPORT); // Server port

	// CONNESSIONE AL SERVER
	if(connect(Csocket, (struct sockaddr *)&sad, sizeof(sad))){
		ErrorHandler( "Failed to connect.\n" );
		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}
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



void invio(int clientSocket, char inputString[BUFFERSIZE], int stringLen){
	if (send(clientSocket, inputString, stringLen, 0) != stringLen) {
				ErrorHandler("send() sent a different number of bytes than expected \n");
				closesocket(clientSocket);
				ClearWinSock();
	}
}

void ricevi(int clientSocket, char buf[BUFFERSIZE]){
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