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

#include <chrono>
#include <thread>
#include <iostream>
#include "input.hpp"


#define GLEEMAIL_PORT 29453
#define MAX_IP_STRING_LENGTH 16 //+1 for terminator in worst-case size
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


unsigned char getRunningIP(char* ipBuffer, const char ipScope) {
	if(ipBuffer == NULL) {
		return 1;
	}

	FILE* hostnameCommand = (ipScope == 'l') ? popen("/bin/hostname -i", "r") : popen("/bin/hostname -I", "r");
	if (hostnameCommand == NULL) {
		return 2;
	}

	fgets(ipBuffer, MAX_IP_STRING_LENGTH, hostnameCommand);
	ipBuffer[MAX_IP_STRING_LENGTH - 1] = '\0';

	pclose(hostnameCommand);
	return 0;
}


int printIPs() {
	unsigned char callFailures = 0;

	char* myIP = new char[16]; //malloc(sizeof(char) * 16);
	unsigned short getIPResult = getRunningIP(myIP, 'l');
	if(getIPResult != 0) {
		std::cout << "Couldn't find local IP! Error: " << getIPResult << std::endl;
		callFailures += 1;
	} else {
		std::cout << "Local IP: " << myIP << std::endl;
	}

	getIPResult = getRunningIP(myIP, 'e');
	if(getIPResult != 0) {
		std::cout << "Couldn't find external IP! Error: " << getIPResult << std::endl;
		callFailures += 1;
	} else {
		std::cout << "External IP: " << myIP << std::endl;
	}

	std::cout << std::endl;
	delete myIP;
	return callFailures;
}


int networkListen(const int socketID, sockaddr* remoteAddress, socklen_t& remoteAddressLength, char incomingBuffer[MAX_BUFFER_SIZE], int& incomingDataLength) {
	if((incomingDataLength = recvfrom(socketID, incomingBuffer, MAX_BUFFER_SIZE, MSG_WAITALL, remoteAddress, &remoteAddressLength)) > 0) {
		std::cout << incomingBuffer << std::endl;
		//sendto(socketID, payload, strlen(payload), MSG_CONFIRM, (struct sockaddr*)&remoteAddress, remoteAddressLength);
	}

	return 0;
}


int main(int argc, const char* argv[]) {
	if(printIPs() > 0) {
		return 1;
	}

	sockInit();

	const int socketID = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketID == -1) {
		return 4;
	}

	sockaddr_in listeningAddress;
	listeningAddress.sin_family = AF_INET;
	listeningAddress.sin_addr.s_addr = INADDR_ANY;
	listeningAddress.sin_port = GLEEMAIL_PORT;

	setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &ONE_FLAG, sizeof(ONE_FLAG));
	if(bind(socketID, reinterpret_cast<sockaddr*>(&listeningAddress), sizeof(listeningAddress)) == -1) {
		return 2;
	}

	bool run = true;
	char userInput[265] = {0};

	Input input;
	std::thread getInput(&Input::processInput, input, std::ref(run));
	getInput.detach();

	sockaddr_in remoteAddress;
	socklen_t remoteAddressLength = sizeof(remoteAddress);

	char incomingBuffer[MAX_BUFFER_SIZE] = {0};
	int incomingDataLength = 0;

	std::cout << "Listening" << std::endl;

	while(run) {
		std::cout << "top" << std::endl;

		//input.processInput(run, userInput);
		//networkListen(socketID, reinterpret_cast<sockaddr*>(&remoteAddress), remoteAddressLength, incomingBuffer, incomingDataLength);

		/*incomingDataLength = recvfrom(socketID, incomingBuffer, MAX_BUFFER_SIZE, MSG_DONTWAIT, reinterpret_cast<sockaddr*>(&listeningAddress), &remoteAddressLength);
		if(incomingDataLength > 0) {
			incomingBuffer[incomingDataLength] = '\0';
			std::cout << incomingBuffer << std::endl;
			//sendto(socketID, payload, strlen(payload), MSG_CONFIRM, (struct sockaddr*)&remoteAddress, remoteAddressLength);
		}*/

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	close(socketID);
	sockQuit();
	return 0;
}