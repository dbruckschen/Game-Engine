#ifndef NETWORK_H
#define NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

void test_network_client(char *address, char *port);
void test_network_server();


#endif // NETWORK_H
