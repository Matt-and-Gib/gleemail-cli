//Thanks to https://stackoverflow.com/a/28031039/4451275 for cross-platform sockets details

#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#include <iostream>


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


int main(int argc, const char* argv[]) {
	std::cout << "Start" << std::endl;

	std::cout << "End" << std::endl;
	return 0;
}