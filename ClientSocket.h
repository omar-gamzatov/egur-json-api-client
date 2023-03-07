#pragma once
#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h> 
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sstream>



class ClientSocket
{
public:
#define MAX_BUNCH_SIZE 1024

	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult;
	SOCKET ConnectSocket;

	char ip;
	char port;
	char recvBuffer[MAX_BUNCH_SIZE];
	int result;

	std::string recievedData;

	ClientSocket();

	int startup();

	int getAddrInfo(const char* ip, const char* port);

	int socketCreate();

	int socketConnect(const char* ip, const char* port);

	int sendRequest(const char* request);

	int shutdownSocket();

	std::string recieveData();

	int shutdownWithMsg(SOCKET socket, ADDRINFO* addr, std::string msg, int result);

	int shutdownWithMsg(ADDRINFO* addr, std::string msg, int result);

	int shutdownWithMsg(std::string msg, int result);

};

#endif // !CLIENTSOCKET_H