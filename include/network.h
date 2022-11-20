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

#define DEFAULT_PORT "27015"

__declspec(dllexport) bool InitializeWinsock(char *ip); 

#endif 
