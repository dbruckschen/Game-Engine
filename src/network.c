#include "network.h"

bool InitializeWinsock(char *ip) {
	int result = 0;
	WSADATA wsa_data = {0};
	
	result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if(result != 0) {
		printf("WSAStartup() failed: %d\n", result);
		return false;
	}

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);

	if(result != 0) {
		printf("getaddrinfo failed: %d\n", result);
		WSACleanup();
		return false;
	}

	struct SOCKET connect_socket = INVALID_SOCKET;

	conncet_socket = socket(ptr->ai_familiy, ptr->ai_socktype, ptr->ai_protocol);

	if(connect_socket == INVALID_SOCKET) {
		printf("error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return false;
	}

	result = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

	// Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message
	
	if(result == SOCKET_ERROR) {
		closesocket(connect_socket);
		connect_socket = INVLAID_SOCKET;
	}

	freeaddrinfo(result);

	if(connect_socket == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}
}
