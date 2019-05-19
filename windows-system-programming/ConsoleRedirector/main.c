#include <stdio.h>
#include <winsock2.h>
#include <windows.h> 
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "handler.h"

SOCKET start_up_server(const char* ip_addr, const char* port);
void shut_down_server(SOCKET listening);

void main() {
	SOCKET listening = start_up_server("127.0.0.1", "45000");
	
	fd_set master;
	FD_ZERO(&master);

	while (1) {
		SOCKET client_conn = accept(listening, NULL, NULL);

		if (client_conn == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			shut_down_server(listening);
			return -1;
		}
		FD_SET(client_conn, &master);

		wprintf(L"Client connected.\n");
		wrap_create_client_handler(client_conn);	
		printf("SERVER: %d\n", client_conn);
	}

	shut_down_server(listening);
	system("pause");
	return 0;
}

SOCKET start_up_server(const char* ip_addr, const char* port) {
	struct addrinfo hints;
	struct addrinfo *server = NULL;
	WSADATA wsData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (iResult != 0) {
		fprintf(stderr, "Can't init winsock\n");
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip_addr, port, &hints, &server);

	SOCKET listening = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
	if (listening == INVALID_SOCKET) {
		fprintf(stderr, "Can't create a socket\n");
		return -1;
	}

	const char OPTION_VALUE = 1;
	setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int));
	setsockopt(listening, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int));

	bind(listening, server->ai_addr, server->ai_addrlen);

	listen(listening, SOMAXCONN);

	return listening;
}

void shut_down_server(SOCKET listening) {
	closesocket(listening);
	WSACleanup();
}