//Thanks to https://stackoverflow.com/a/28031039/4451275 for cross-platform sockets details

#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>

	typedef int SOCKET;
#endif

#include <iostream>

#define GLEEMAIL_PORT 29453
const static int ONE_FLAG = 1; //Necessary for setsockopt
const static constexpr unsigned short MAX_BUFFER_SIZE = 512;


int sockInit(void) {
#ifdef _WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1,1), &wsa_data);
#else
	return 0;
#endif
}


int sockClose(SOCKET sock) {
	int status = 0;

#ifdef _WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0) {
		status = closesocket(sock);
	}
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) {
		status = close(sock);
	}
#endif

  return status;
}


int sockQuit(void) {
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}


int networkListen(const int socketID, sockaddr* remoteAddress, socklen_t& remoteAddressLength, char incomingBuffer[MAX_BUFFER_SIZE], int& incomingDataLength) {
	if((incomingDataLength = recvfrom(socketID, incomingBuffer, MAX_BUFFER_SIZE, MSG_WAITALL, remoteAddress, &remoteAddressLength)) > 0) {
		std::cout << incomingBuffer << std::endl;
		//sendto(socketID, payload, strlen(payload), MSG_CONFIRM, (struct sockaddr*)&remoteAddress, remoteAddressLength);
	}

	return 0;
}


int main(int argc, const char* argv[]) {
	sockInit();

	const int socketID = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketID == -1) {
		return 4;
	}

	sockaddr_in listeningAddress;
	listeningAddress.sin_family = AF_INET;
	listeningAddress.sin_addr.s_addr = INADDR_ANY;
	listeningAddress.sin_port = GLEEMAIL_PORT;

	setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, &ONE_FLAG, sizeof(ONE_FLAG));
	if(bind(socketID, reinterpret_cast<sockaddr*>(&listeningAddress), sizeof(listeningAddress)) == -1) {
		return 2;
	}

	sockaddr_in remoteAddress;
	socklen_t remoteAddressLength = sizeof(remoteAddress);

	char incomingBuffer[MAX_BUFFER_SIZE] = {0};
	int incomingDataLength = 0;

	while(1) {
		networkListen(socketID, reinterpret_cast<sockaddr*>(&remoteAddress), remoteAddressLength, incomingBuffer, incomingDataLength);
	}

	sockQuit();
	return 0;
}