#ifndef NETWORK_H
#define NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

__declspec(dllexport) void test_network_client(char *address, char *port);
__declspec(dllexport) void test_network_server();

#endif // NETWORK_H
