#pragma once

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <memory.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <winternl.h>

//beomu
#include <uchar.h>

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <exception>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdint>

#include "network_headers/ip.h"

