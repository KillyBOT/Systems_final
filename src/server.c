#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"
#include "network.h"

void subServer(int cSocket, int player, int readPipe);

int main(int argc, char* argv[]){

	int lSocket; //Listening socket
	int cSocket[MAX_PLAYERS]; //Client sockets for each client
	int subServer_count; //Number of sub serverss
	int pPipe[MAX_PLAYERS][2]; //Pipes for communicating to each player 
	struct gameState* g = createState();
	int f;
	int pConnected = 0;
	int acceptConnects = 1;
	int runServer = 1;
	int dir;

	fd_set read_fds; //File descriptor to read from

	lSocket = server_setup();

	while(pConnected < MAX_PLAYERS && acceptConnects && pConnected < 2){

		cSocket[pConnected] = server_connect(lSocket);

		write(cSocket[pConnected], &pConnected, sizeof(pConnected));
		addPlayer2(g,pConnected);

		pipe(pPipe[pConnected]);
		//printf("%d %d\n", pPipe[pConnected][0], pPipe[pConnected][1]);

		f = fork();
		if(f == 0){
			close(pPipe[pConnected][PIPE_WRITE]);
			subServer(cSocket[pConnected], pConnected, pPipe[pConnected][PIPE_READ] ); //This is the write port
		} else {
			close(pPipe[pConnected][PIPE_READ]);
			close(cSocket[pConnected]);
			cSocket[pConnected] = server_connect(lSocket);
			//subServer_count++;
			pConnected++;
		}

		//acceptConnects = 0;
	}

	for(int n = 0; n < pConnected; n++){
		//printState(g);
		write(pPipe[n][PIPE_WRITE],g,sizeof(struct gameState));
	}


	//fd_set client_fds;// The file descriptors for each client

	while(runServer){
		for(int n = 0; n < pConnected; n++){
			recv(cSocket[n],&dir,sizeof(int),MSG_WAITALL);

			//printf("Client %d says go in direction %d\n", n, dir);

			changePlayerDir(g,n,dir);
		}

		//printState(g);
		updateState(g);
		//printState(g);

		for(int n = 0; n < pConnected; n++) {
			write(pPipe[n][PIPE_WRITE],g,sizeof(struct gameState));
		}

		runServer = 0;
		for(int x = 0; x < MAX_PLAYERS; x++){
			if(g->pData[x] > 0 && g->pData[x] != PLAYER_STATE_DEAD){
				runServer = 1;
			}
		}

	}
	printf("Game over!\n");
	printLeaderBoard(g);

	for(int n = 0; n < pConnected; n++){
		close(cSocket[n]);
		close(pPipe[n][PIPE_WRITE]);
	}

	deleteState(g);

	return 0;

}

void subServer(int cSocket, int player, int readPipe){

	struct gameState g;
	int keepRunning = 1;

	while(keepRunning){
		read(readPipe, &g, sizeof(g));
		write(cSocket, &g, sizeof(g));

		keepRunning = 0;
		for(int x = 0; x < MAX_PLAYERS; x++){
			if(g.pData[x] > 0 && g.pData[x] != PLAYER_STATE_DEAD){
				keepRunning = 1;
			}
		}
	}


	close(cSocket);
	close(readPipe);

	exit(0);
}