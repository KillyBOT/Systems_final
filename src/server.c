#include "snakeEater.h"

int main(int argc, char* argv[]){

	struct sockaddr_in address;
	int serverFD, testSocket, readVal;
	int opt = 1;
	int addrlen = sizeof(address);
	char readBuffer[1024] = {0};
	char* testMsg = "From server";

	serverFD = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORTNUM);

	bind(serverFD, (struct sockaddr*)&address, sizeof(address));

	listen(serverFD, 3);

	testSocket = accept(serverFD, (struct sockaddr*)&address, (socklen_t*)&addrlen);

	readVal = read(testSocket, readBuffer, 1024);
	printf("Recieved: %s\n", readBuffer);
	send(testSocket, testMsg, strlen(testMsg), 0);
	printf("Sent message to client\n");
	return 0;

}