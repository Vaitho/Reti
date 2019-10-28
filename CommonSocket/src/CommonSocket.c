#include "CommonSocket.h"

#define BUFFERSIZE 512

void ErrorHandler(char *errorMessage){
    printf("%s",errorMessage);
}
/////////////////////////////////////////////////////////////////

void ClearWinSock(){
    WSACleanup();
}
/////////////////////////////////////////////////////////////////

int checkPort(int port){
	if(port < 1024 || port > 65535){
		return 0;
	}else{
		return 1;
	}
}
/////////////////////////////////////////////////////////////////

void startWSAS() {
#if defined WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	if(result != 0){
		ErrorHandler("Error at WSAStartup()\n");
	}else{
		printf("WSAStartup() okay! \n");
	}
}
/////////////////////////////////////////////////////////////////

int createSocket(int MySocket){
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySocket < 0) {
		ErrorHandler("socket creation failed.\n");
		ClearWinSock();
		return -1;
	}else{
		printf("Socket creation accept! \n");
		return MySocket;
	}
}
/////////////////////////////////////////////////////////////////
void bindSocket(int MySocket, char* ip, int port){
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(ip);
	sad.sin_port = htons(port);
	if(bind(MySocket, (struct sockaddr*) &sad, sizeof(sad))<0){
		ErrorHandler("bind() failed.\n");
		closesocket(MySocket);
		ClearWinSock();
	}else{
		printf("bind() it's okay!\n");
	}
}
/////////////////////////////////////////////////////////////////
void listenSocket(int MySocket, int len){
	if(listen(MySocket, len)){
		ErrorHandler("listen() failed.\n");
		closesocket(MySocket);
		ClearWinSock();
	}else{
		printf("listen() okay! \n");
	}
}
/////////////////////////////////////////////////////////////////
int acceptSocket(int MySocket){
	struct sockaddr_in cad; // structure for the client address
	int clientLen; // the size of the client address
	int clientSocket; // socket descriptor for the client
	clientLen = sizeof(cad); // set the size of the client address
	if((clientSocket = accept(MySocket,(struct sockaddr *)&cad, &clientLen)) < 0){
		ErrorHandler("accept() failed.\n");
		// CHIUSURA DELLA CONNESSIONE
		closesocket(MySocket);
		ClearWinSock();
		return 0;
	}else{
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
		return clientSocket;
	}
}
/////////////////////////////////////////////////////////////////
void invio(int clientSocket,char inputString[BUFFERSIZE],int stringLen){
	if (send(clientSocket, inputString, stringLen, 0) != stringLen) {
				ErrorHandler("send() sent a different number of bytes than expected \n");
				closesocket(clientSocket);
				ClearWinSock();
	}
}
/////////////////////////////////////////////////////////////////

void ricevi(int clientSocket,char buf[BUFFERSIZE]){
	int bytesRcvd;
	int totalBytesRcvd = 0;

	printf("Received: ");
		while (totalBytesRcvd == 0){
			if ((bytesRcvd = recv(clientSocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
				ErrorHandler("recv() failed or connection closed prematurely \n");
				closesocket(clientSocket);
				ClearWinSock();
			}
			totalBytesRcvd += bytesRcvd;
			buf[bytesRcvd] = '\0';
			printf("%s", buf);
			printf("\n");
		}
}
/////////////////////////////////////////////////////////////////

int connectClient(int socket, char* ip, int port){
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(ip);
	sad.sin_port = htons(port);
	if(connect(socket, (struct sockaddr *)&sad, sizeof(sad))){
		return -1;
	}else{
		return 0;
	}
}
