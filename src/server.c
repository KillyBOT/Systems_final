#include "snakeEater.h"
#include "game.h"
#include "network.h"

void subServer(int cSocket);

int main(int argc, char* argv[]){

	int lSocket; //Listening socket
	int cSocket; //Client sockets for each client
	int subServer_count; //Number of sub serverss
	char buffer[512];
	int pPipe[MAX_PLAYERS][2]; //Pipes for communicating to each player 
	struct gameCommand* gC = (struct gameCommand*)calloc(sizeof(struct gameCommand),1);
	int f;
	int pConnected = 0;
	int acceptConnects = 1;
	int runServer = 1;

	fd_set read_fds; //File descriptor to read from

	lSocket = server_setup();

	while(pConnected < MAX_PLAYERS && acceptConnects){

		cSocket[pConnected] = server_connect(lSocket);

		f = fork();
		if(f == 0){
			subServer(cSocket[pConnected]); //This is the write port
		} else {
			close(cSocket[pConnected]);
			cSocket[pConnected] = server_connect(lSocket);
			subServer_count++;
			pConnected++;

			acceptConnects = 0;
		}
	}

	while(runServer){
		
	}

	for(int n = 0; n < pConnected){
		close(cSocket[n]);
	}

}

void subServer(int cSocket){
	struct gameCommand* gC = (struct gameCommand*)calloc(sizeof(struct gameCommand),1);
	char buffer[512];

	fgets(buffer, sizeof(buffer),stdin);
	gC->cType = (int)strtol(buffer,NULL,10);

	write(cSocket, gC, sizeof(gC));
	close(cSocket);
	exit(0);
}