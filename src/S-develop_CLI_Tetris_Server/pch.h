#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include <thread>
#include <mutex>
#include <condition_variable>

extern "C" {
#include <conio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
}

#include "network_headers/ip.h"

#pragma comment(lib, "Ws2_32.lib")

