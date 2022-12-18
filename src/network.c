#include "network.h"

bool InitializeWinsock(int major_version, int minor_version, WSADATA *wsa_data) {
	int error = 0;
	
	error = WSAStartup(MAKEWORD(major_version, minor_version), wsa_data);
	if(error != 0) {
		printf("WSAStartup() failed: %d\n", error);
		return false;
	}
	else {
		printf("WSAStartup() success\n");
		return true;
	}
}

bool CreateClient(const char *serv_ip, const char *serv_port, SOCKET *connect_socket) {
	int error = 0;
	
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_protocol = IPPROTO_TCP;

	error = getaddrinfo(serv_ip, serv_port, &hints, &result);

	if(error != 0) {
		printf("getaddrinfo failed: %d, function: %s\n", error, __FUNCTION__);
		return false;
	}
	else {
		printf("getaddrinfo success, function %s\n", __FUNCTION__);
	}

	*connect_socket = INVALID_SOCKET;

	// Attempt to connect to an address until one succeds
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create Socket
		*connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(*connect_socket == INVALID_SOCKET) {
			printf("error at socket(): %ld, function: %s\n", WSAGetLastError(), __FUNCTION__);
			freeaddrinfo(result);
			
			return false;
		}
		else {
			printf("socket() success\n");
		}

		// Connect to server
		error = connect(*connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if(error == SOCKET_ERROR) {
			int connect_error = WSAGetLastError();
			printf("error code: %d\n", connect_error);

			closesocket(*connect_socket);
			*connect_socket = INVALID_SOCKET;
			printf("connect() error, function: %s\n", __FUNCTION__);
		}
		else {
			printf("connect() success\n");
			break;
		}
	}

	freeaddrinfo(result);

	if(*connect_socket == INVALID_SOCKET) {
		return false;
	}
	else {
		return true;
	}
}

bool CreateServer(const char *serv_port, SOCKET *listen_socket) {
	int error = 0;

	struct addrinfo *servinfo = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	error = getaddrinfo(NULL, serv_port, &hints, &servinfo);

	// Print host information
	char hostbuf[256];
	gethostname(hostbuf, 256);
	printf("hostname server: %s\n", hostbuf);
	char ip[128];
	
	struct sockaddr_in *localaddr = (struct sockaddr_in *)servinfo->ai_addr;
	inet_ntop(AF_INET, &localaddr->sin_addr, ip, 128);
	printf("server ip: %s\n", ip);
	printf("server port: %d\n", localaddr->sin_port);
	
	if(error != 0) {
		printf("getaddrinfo() failed: %d, function: %s\n", error, __FUNCTION__);
		return false;
	}
	else {
		printf("getaddrinfo() success\n");
	}

	*listen_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	// make the listen_socket not block so when we call listen() we can still render stuff etc.
	u_long mode = 1; //non blocking
	ioctlsocket(*listen_socket, FIONBIO, &mode);

	if(*listen_socket == INVALID_SOCKET) {
		closesocket(*listen_socket);
		*listen_socket = INVALID_SOCKET;
		printf("socket() error, function: %s\n", __FUNCTION__);
		return false;
	}
	else {
		printf("socket() success, function: %s\n", __FUNCTION__);
	}
	
	if(bind(*listen_socket, servinfo->ai_addr, (int)servinfo->ai_addrlen) == SOCKET_ERROR) {
		closesocket(*listen_socket);
		printf("bind() error, function %s\n", __FUNCTION__);
		return false;
	}
	else {
		printf("bind() success, function %s\n", __FUNCTION__);
	}

	error = listen(*listen_socket, 2);
	if(error != 0) {
		closesocket(*listen_socket);
		printf("listen() error, function: %s\n", __FUNCTION__);
		return false;
	}
	else {
		printf("listen() success, function: %s\n", __FUNCTION__);
	}

	return true;
}
