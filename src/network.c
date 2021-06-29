#include "network.h"

void test_network_client(char *address, char *port)
{
    WSADATA wsa_data;
    SOCKET sockt;
    struct addrinfo addr_info = {0};
    struct addrinfo *result = 0;

    if(WSAStartup(MAKEWORD(1, 1), &wsa_data) == 0)
    {
	printf("WSAStartup successful!\n");
    }
    else
    {
	printf("WSAStrtup failed!\n");
    }

    addr_info.ai_family = AF_UNSPEC;
    addr_info.ai_socktype = SOCK_STREAM;
    addr_info.ai_protocol = IPPROTO_TCP;

    if(getaddrinfo(address, port, &addr_info, &result) != 0)
    {
	printf("getaddrinfo failed!\n");
    }
    else
    {
	printf("getaddrinfo successful!\n");
    }
    
    sockt = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if(sockt == INVALID_SOCKET)
    {
	printf("socket creation failed!\n");
    }
    else
    {
	printf("socket creation successful!\n");
    }

    if(connect(sockt, result->ai_addr, (int)result->ai_addrlen) == 0)
    {
	printf("connect successful!\n");
    }
    else
    {
	printf("connect failed!\n");
	closesocket(sockt);
	sockt = INVALID_SOCKET;
    }
}

void test_network_server()
{

}
