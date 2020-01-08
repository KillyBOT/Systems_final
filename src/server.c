#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"
#include "network.h"

#define PIPE_WRITE_FD 5

void subServer(int cSocket, int player, int readPipe);

int main(int argc, char* argv[]){

	int lSocket; //Listening socket
	int cSocket[MAX_PLAYERS]; //Client sockets for each client
	int subServer_count; //Number of sub serverss
	char buffer[512];
	int pPipe[MAX_PLAYERS][2]; //Pipes for communicating to each player 
	struct gameCommand* gC = (struct gameCommand*)calloc(sizeof(struct gameCommand),1);
	struct gameState* g = createState();
	int f;
	int pConnected = 0;
	int acceptConnects = 1;
	int runServer = 1;

	fd_set read_fds; //File descriptor to read from

	lSocket = server_setup();

	while(pConnected < MAX_PLAYERS && acceptConnects){

		cSocket[pConnected] = server_connect(lSocket);
		addPlayer(g);

		f = fork();
		pipe(pPipe[pConnected]);
		if(f == 0){
			close(pPipe[pConnected][1]);
			subServer(cSocket[pConnected],pConnected,pPipe[pConnected][0]); //This is the write port
		} else {
			close(cSocket[pConnected]);
			close(pPipe[pConnected][0]);
			cSocket[pConnected] = server_connect(lSocket);
			subServer_count++;
			pConnected++;

			acceptConnects = 0;
		}
	}

	while(runServer){
		for(int n = 0; n < pConnected; n++){
			write(pPipe[n][1],g,sizeof(struct gameState));
		}
		break;

	}

	for(int n = 0; n < pConnected; n++){
		close(cSocket[n]);
		close(pPipe[n][1]);
	}

	deleteState(g);

}

void subServer(int cSocket, int player, int readPipe){

	struct gameState* g;

	write(cSocket, &player, sizeof(int));

	read(readPipe, g, sizeof(g));
	write(cSocket, g, sizeof(g));

	close(cSocket);
	close(readPipe);
	
	exit(0);
}