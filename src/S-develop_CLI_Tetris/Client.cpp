#include "pch.h"
#include "Client.h"

using namespace std;

Client::Client() {
	WSAStartup(MAKEWORD(2, 2), &wsaData_);

    {  
        adrInfo_.ip_.resize(sizeof(uint32_t));
        adrInfo_.ip_.at(0) = 127;
        adrInfo_.ip_.at(1) = 0;
        adrInfo_.ip_.at(2) = 0;
        adrInfo_.ip_.at(3) = 1;

        adrInfo_.port_ = 8986;
    }
}

Client::~Client() {
    closesocket(socket_);
    WSACleanup();

}

bool Client::Connect(Client::ADR_INFO& adrInfo) {
    try {  
        socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
        if (socket_ == INVALID_SOCKET)  
            throw runtime_error("Failed to create socket");  

        //ip set
        serverAddrInfo_.sin_family = AF_INET;
        
        memcpy(&serverAddrInfo_.sin_addr.S_un.S_addr, adrInfo.ip_.data(), adrInfo.ip_.size());
        serverAddrInfo_.sin_port = htons(adrInfo.port_);

        DWORD timeout = 3000;//no
        setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        
        if (::connect(socket_, reinterpret_cast<sockaddr*>(&serverAddrInfo_), sizeof(serverAddrInfo_)) < 0)
            throw runtime_error("Failed to call connect");

        return true;
    }  
    catch (const exception& e) {  
        cerr << "<Init> " << e.what() << endl;
    }  

    return false;
}

bool Client::Connect() {
    return Connect(adrInfo_);
}