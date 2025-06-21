#include "pch.h"
#include "Server.h"

using namespace std;

Server::Server(const std::string& ipAddress, int port)
	: ipAddress_(ipAddress)
	, port_(port)
	, isRunning_(false)
{
	if (!Initialize()) throw runtime_error("Failed to initialize server.");
}

Server::~Server()
{
	Stop();
}

bool Server::Initialize() {
	WSADATA wsaData;
	addrinfo* addrResult = nullptr;

	try {
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
			throw runtime_error("WSAStartup failed: ");

		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		result = getaddrinfo(ipAddress_.c_str(), std::to_string(port_).c_str(), &hints, &addrResult);
		if (result != 0)
			throw runtime_error("Failed to get addr info");

		listenSocket_ = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
		if (listenSocket_ == INVALID_SOCKET)
			throw runtime_error("Failed to create socket");

		result = bind(listenSocket_, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
			throw runtime_error("Faield to bind socket");

	}
	catch (const std::exception& e) {
		std::cerr << "Exception during initialization: " << e.what() << std::endl;

		freeaddrinfo(addrResult);
		closesocket(listenSocket_);
		WSACleanup();

		return false;
	}

	return true;
}

bool Server::Start(){
	if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
		closesocket(listenSocket_);
		WSACleanup();
		return false;
	}

	isRunning_ = true;
	listenThread_ = std::thread(&Server::ListenForClients, this);
	return true;
}

void Server::ListenForClients() {
	while (isRunning_)
	{
		SOCKET clientSocket = accept(listenSocket_, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			if (isRunning_)
			{
				std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
			}
			continue;
		}

		std::thread clientThread(&Server::HandleClient, this, clientSocket);
		clientThread.detach();
	}
}

void Server::HandleClient(SOCKET clientSocket)
{
	char buffer[1024];
	int result;

	while (isRunning_)
	{
		result = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (result > 0)
		{
		}
		else if (result == 0 || WSAGetLastError() == WSAECONNRESET)
		{

			break;
		}
		else
		{
			std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
			break;
		}
	}

	closesocket(clientSocket);
}

void Server::Stop()
{
	isRunning_ = false;

	if (listenSocket_ != INVALID_SOCKET)
	{
		closesocket(listenSocket_);
		listenSocket_ = INVALID_SOCKET;
	}

	if (listenThread_.joinable())
		listenThread_.join();

	WSACleanup();
	std::cout << "Server stopped" << std::endl;
}
