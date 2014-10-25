#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment ( lib, "Ws2_32.lib" )
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSize;
	SOCKADDR_IN servAdr, clntAdr;
	if (argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1)
	{
		clntAdrSize = sizeof(clntAdr);
		strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAdr, &clntAdrSize);
		sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAdr, clntAdrSize);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
