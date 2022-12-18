#ifndef NETWORK_H
#define NETWORK_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include "common.h"

#define DEFAULT_PORT "43970"

struct Server {
	SOCKET connect_socket;
	SOCKET listen_socket;
	WSAPOLLFD pollfd;
};

// calls wsa startup function and writes result into *wsa_data
__declspec(dllexport) bool InitializeWinsock(int major_version, int minor_version, WSADATA *wsa_data);
// creates a connect socket and calls connect on a specified ip and port
__declspec(dllexport) bool CreateClient(const char *serv_ip, const char *serv_port, SOCKET *socket);
__declspec(dllexport) bool CreateServer(const char *serv_port, SOCKET *listen_socket);


#endif 
