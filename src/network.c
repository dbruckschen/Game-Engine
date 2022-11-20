#include "network.h"

bool InitializeWinsock(char *ip) {
	int error = 0;
	WSADATA wsa_data = {0};
	
	error = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if(error != 0) {
		printf("WSAStartup() failed: %d\n", error);
		return false;
	}

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	error = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);

	if(error != 0) {
		printf("getaddrinfo failed: %d\n", error);
		WSACleanup();
		return false;
	}

	SOCKET connect_socket = INVALID_SOCKET;

	connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if(connect_socket == INVALID_SOCKET) {
		printf("error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return false;
	}

	error = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

	// Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message
	
	if(error == SOCKET_ERROR) {
		closesocket(connect_socket);
		connect_socket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if(connect_socket == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}

	return true;
}
