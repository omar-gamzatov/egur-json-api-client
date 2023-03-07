#include "ClientSocket.h"


	ClientSocket::ClientSocket() {
		this->addrResult = NULL;
		this->ConnectSocket = INVALID_SOCKET;

		ZeroMemory(&this->hints, sizeof(this->hints));
		this->hints.ai_family = AF_INET;
		this->hints.ai_socktype = SOCK_STREAM;
		this->hints.ai_protocol = IPPROTO_TCP;
	}

	int ClientSocket::startup() {
		this->result = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
		if (this->result != 0) {
			std::cout << "WSAStartup failed, result = " << this->result << std::endl;
			return 1;
		} else return 0;
	}

	int ClientSocket::getAddrInfo(const char* ip, const char* port) {
		this->result = getaddrinfo(ip, port, &this->hints, &this->addrResult);
		if (this->result != 0) {
			return shutdownWithMsg("getaddrinfo failed with error", 1);
		} else return 0;
	}

	int ClientSocket::socketCreate() {
		this->ConnectSocket = socket(this->addrResult->ai_family, this->addrResult->ai_socktype, addrResult->ai_protocol);
		if (this->ConnectSocket == INVALID_SOCKET) {
			return shutdownWithMsg(this->addrResult, "Socket creation failed", 1);
		} else return 0;
	}

	int ClientSocket::socketConnect(const char* ip, const char* port) {
		this->startup();
		this->getAddrInfo(ip, port);
		this->socketCreate();
		this->result = connect(this->ConnectSocket, this->addrResult->ai_addr, (int)this->addrResult->ai_addrlen);
		if (this->result == SOCKET_ERROR) {
			return shutdownWithMsg(this->ConnectSocket, this->addrResult, "Unable connect to server", 1);
		} else return 0;
	}

	int ClientSocket::shutdownSocket() {
		this->result = shutdown(this->ConnectSocket, SD_SEND);
		if (this->result == SOCKET_ERROR) {
			return shutdownWithMsg(this->ConnectSocket, this->addrResult, "Shutdown error: " + this->result, 1);
		} else return 0;
	}

	std::string  ClientSocket::recieveData() {
		struct sockaddr_in SenderAddr;
		int SenderAddrSize = sizeof(SenderAddr);
		do
		{
			ZeroMemory(this->recvBuffer, sizeof(this->recvBuffer));
			this->result = recvfrom(this->ConnectSocket, this->recvBuffer, MAX_BUNCH_SIZE, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

			if (this->result == 0)
				return "Response: 0";

			if (this->result > 0) {
				//std::cout << "Received " << this->result << " bytes" << std::endl;
				this->recievedData += this->recvBuffer;
			}
			else {
				return "Recv failed with error";
			}
		} while (this->result == MAX_BUNCH_SIZE);
		return this->recievedData;
	}


	int ClientSocket::sendRequest(const char* request) {
		//this->recievedData = request;
		//this->recievedData += "\n";

		const char* sendBuffer = request;
		this->result = send(this->ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
		if (this->result == SOCKET_ERROR) {
			return shutdownWithMsg(this->ConnectSocket, this->addrResult, "Send failed, error: " + this->result, 1);
		}
		//std::cout << "Sent: " << result << " bytes" << std::endl;
		
		return 0;
	}

	int ClientSocket::shutdownWithMsg(SOCKET socket, ADDRINFO* addr, std::string msg, int result) {
		std::cout << msg << std::endl;
		closesocket(socket);
		socket = INVALID_SOCKET;
		freeaddrinfo(addr);
		WSACleanup();
		return result;
	}

	int ClientSocket::shutdownWithMsg(ADDRINFO* addr, std::string msg, int result) {
		std::cout << msg << std::endl;
		freeaddrinfo(addr);
		WSACleanup();
		return result;
	}

	int ClientSocket::shutdownWithMsg(std::string msg, int result) {
		std::cout << msg << std::endl;
		WSACleanup();
		return result;
	}
