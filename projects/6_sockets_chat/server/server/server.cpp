#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h> // Use stdio.h for perror
#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 1024
#define PORT_ID 5432

bool running = true;

//std::vector<SOCKET> clients;
//void handleClient(SOCKET clientSocket) {
//    char buffer[BUFFER_SIZE];
//    while (running) {
//        memset(buffer, 0, sizeof(buffer));
//        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//        if (bytesReceived <= 0) {
//            std::cout << "Connection lost with the client." << std::endl;
//            closesocket(clientSocket);
//            for (auto it = clients.begin(); it != clients.end(); ++it) {
//                if (*it == clientSocket) { clients.erase(it); running = false; }
//            }
//            return;
//        }
//
//        char message;
//        if (message == 'q') {
//            std::cout << "Exit." << std::endl;
//            closesocket(clientSocket);
//            for (auto it = clients.begin(); it != clients.end(); ++it) {
//                if (*it == clientSocket) { clients.erase(it); running = false; }
//            }
//            return;
//        }
//        for (const auto& otherClient : clients) {
//            if (otherClient != clientSocket)
//                send(otherClient, &message, 1, 0);
//        }
//    }
//}

int main() {
    // Initialize WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup");
        return 1;
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        WSACleanup(); // Cleanup WinSock on error
        return 1;
    }

    // Set up server address
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_ID);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        closesocket(sockfd); // Close socket on error
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening for connection." << std::endl;

    // Accept a connection
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
        perror("accept");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Data transfer loop
    char buffer[BUFFER_SIZE];
    while (true) {
        int n = recv(clientfd, buffer, sizeof(buffer), 0);
        if (n == -1) {
            perror("recv");
            break;
        }
        else if (n == 0) {// Client disconnected
            break;
        }

        n = send(clientfd, buffer, n, 0);
        if (n == -1) {
            perror("send");
            break;
        }
    }

    // Close sockets and cleanup
    closesocket(clientfd);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}