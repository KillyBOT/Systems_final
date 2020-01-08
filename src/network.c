#include "snakeEater.h"
#include "network.h"
#include "game.h"

void error_check(int i, char* s){
	if(i < 0){
		printf("%s error %d: %s\n", s, errno, strerror(errno));
		exit(1);
	}
}

int server_setup(){
	int sd; //Server descriptor

	sd = socket(AF_INET, SOCK_STREAM, 0);
	error_check(sd, "Server socket");
	printf("Server socket created\n");

	struct addrinfo * hints, * results;
	hints = (struct addrinfo*)calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;  //IPv4 address
	hints->ai_socktype = SOCK_STREAM;  //TCP socket
	hints->ai_flags = AI_PASSIVE;  //Use all valid addresses
	getaddrinfo(NULL, PORT, hints, &results); //NULL means use local address

	error_check( bind(sd, results->ai_addr, results->ai_addrlen), "Server bind");
	printf("Server socket bounded\n");

	error_check( listen(sd, 8), "Server listen" );
	printf("Server socket now listening...\n");

	free(hints);
	freeaddrinfo(results);

	return sd;
}

int server_connect(int sd){
	int cSock; //Client socket
	char cAddrStr[INET_ADDRSTRLEN];
	socklen_t sockSize;
	struct sockaddr_storage cAddr; //Client address

	sockSize = sizeof(cAddr);
	cSock = accept(sd, (struct sockaddr*)&cAddr, &sockSize);
	error_check(cSock, "Server accept");

	inet_ntop(AF_INET, &(((struct sockaddr_in*)&cAddr)->sin_addr), cAddrStr, INET_ADDRSTRLEN);

	printf("Connected to client [%s]\n",cAddrStr);

	return cSock;

}

int client_setup(char* server){
	int sd; //File descriptor to return

	sd = socket(AF_INET, SOCK_STREAM, 0);
	error_check(sd, "Client socket");

	struct addrinfo *hints, *results;
	hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;  //IPv4
	hints->ai_socktype = SOCK_STREAM;  //TCP socket
	getaddrinfo(server, PORT, hints, &results);

	error_check( connect(sd, results->ai_addr, results->ai_addrlen), "Client connect" );

	free(hints);
	freeaddrinfo(results);

	return sd;

}