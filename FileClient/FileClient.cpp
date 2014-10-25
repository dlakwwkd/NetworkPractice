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
	SOCKET hSocket;
	FILE* fp;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAdr;
	if (argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fopen_s(&fp, "456.pptx", "wb");
	if (fp == NULL)
		ErrorHandling("file open Error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		printf("%d\n", error);
	}
	while (1)
	{
		readCnt = recv(hSocket, buf, BUF_SIZE, 0);
		if (readCnt == 0)
			break;
		else if (readCnt == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			printf("error : %d\n", error);
			break;
		}
		else
			fwrite((void*)buf, 1, readCnt, fp);
	}

	puts("Received file data");
	send(hSocket, "Thank you", 10, 0);
	fclose(fp);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
