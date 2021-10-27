#ifdef _WIN32
	#include <winsock2.h>
	#include <Ws2tcpip.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#include <iostream>


int main(const unsigned short argc, const char* argv[]) {
	std::cout << "Start" << std::endl;

	return 0;
}