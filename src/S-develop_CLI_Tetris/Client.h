#pragma once

class Client {
protected:
	struct ADR_INFO {
		std::vector<uint8_t> ip_{};
		DWORD port_{};
	};

private:
	WSADATA wsaData_{};
	SOCKET socket_{};
	SOCKADDR_IN serverAddrInfo_{};

	ADR_INFO adrInfo_{};

protected:
	bool Connect(ADR_INFO& addr);

public:
	explicit Client();
	~Client();

	bool Connect();
};

