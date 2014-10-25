#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment ( lib, "Ws2_32.lib" )
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;
	
	if (argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fopen_s(&fp, "123.pptx", "rb");
	if (fp == NULL)
		ErrorHandling("file open Error");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSize = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);

	int i = 0;
	while (1)
	{
		int ret = 0;
		printf("%d\n",i++);
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			ret = send(hClntSock, buf, readCnt, 0);
			printf("%d\n", ret);
			break;
		}
		ret = send(hClntSock, buf, BUF_SIZE, 0);
		if (ret == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			printf("%d\n", error);
		}
		printf("%d\n", ret);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from client: %s \n", buf);

	fclose(fp);
	closesocket(hClntSock);
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
