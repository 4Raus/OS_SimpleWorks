#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>
#pragma comment(lib, "ws2_32.lib")

#define PORT_ID 65000

std::mutex mut;
bool isDisconnectOrError = false;

void receiveMessages(SOCKET sock) {
	char buffer[1024];
	while (1) {
		int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
		if (bytesReceived == 0) {
			isDisconnectOrError = true;
			std::cout << "Server closed the connecting." << std::endl;
			break;
		}
		else if (bytesReceived == SOCKET_ERROR) {
			isDisconnectOrError = true;
			std::cerr << "Error in receiving data from server." << std::endl;
			break;
		}
	}
}

int main() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		std::cerr << "Can't initialize WinSocket." << std::endl;
		return -1;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Can't create socket." << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT_ID);
	inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

	int result = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (result == SOCKET_ERROR) {
		std::cerr << "Can't connect to the server." << std::endl;
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	std::string name;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);
	send(sock, name.c_str(), name.size(), 0);
	std::string message;

	std::thread receiver(receiveMessages, sock);
	char buffer[1024];
	while (message != "exit" && !isDisconnectOrError) {
		if (isDisconnectOrError) break;
		std::cout << "Enter your message: ";
		std::getline(std::cin, message);
		send(sock, message.c_str(), message.size(), 0);
	}
	receiver.join();
	std::cout << "Disconnecting" << std::endl;
	closesocket(sock);
	WSACleanup();

	return 0;
}