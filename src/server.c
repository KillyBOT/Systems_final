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
	int pDir[MAX_PLAYERS]; //The buffer for the direction of each player. -1 means they're dead
	int pToAccept; //How many players to accept connections from
	int pConnected; //How many players are connected currently
	int dir;
	int f;

	int acceptConnects; //Should the server accept connects
	int runServer; //Should the main server loop run
	int rerunServer = 1; //Should the server be repeated
	char buffer[4]; //Just an input buffer

	if(argc > 1) pToAccept = atoi(argv[1]); //Set the max number of players that can connect to argv[1]
	else pToAccept = 2;

	lSocket = server_setup();

	while(rerunServer){

		printf("Waiting for %d players to connect...\n", pToAccept);

		pConnected = 0;
		acceptConnects = 1;
		runServer = 1;

		for(int p = 0; p < MAX_PLAYERS; p++){
			pDir[p] = -1;
		}

		while(pConnected < MAX_PLAYERS && acceptConnects && pConnected < pToAccept){

			cSocket[pConnected] = server_connect(lSocket);

			write(cSocket[pConnected], &pConnected, sizeof(pConnected));
			pDir[pConnected] = PLAYER_STATE_ALIVE;

			pipe(pPipe[pConnected]);

			f = fork();
			if(f == 0){
				close(pPipe[pConnected][PIPE_WRITE]);
				subServer(cSocket[pConnected], pConnected, pPipe[pConnected][PIPE_READ] ); //This is the write port
			} else {
				close(pPipe[pConnected][PIPE_READ]);
				close(cSocket[pConnected]);
				cSocket[pConnected] = server_connect(lSocket);
				pConnected++;
			}

		}

		for(int n = 0; n < pConnected; n++){
			write(pPipe[n][PIPE_WRITE],pDir,sizeof(pDir));
		}

		while(runServer){
			for(int n = 0; n < pConnected; n++){
				recv(cSocket[n],&dir,sizeof(dir),MSG_WAITALL);

				//printf("Client %d says go in direction %d\n", n, dir);

				pDir[n] = dir;
			}

			for(int n = 0; n < pConnected; n++) {
				write(pPipe[n][PIPE_WRITE],pDir,sizeof(pDir));
			}

			runServer = 0;
			for(int x = 0; x < MAX_PLAYERS; x++){
				if(!(pDir[x] < 0)){
					runServer = 1;
					break;
				}
			}

		}

		printf("Game over!\n");

		for(int n = 0; n < pConnected; n++){
			close(cSocket[n]);
			close(pPipe[n][PIPE_WRITE]);
		}

		printf("Do you want to start another game instance? [Y/N]\n");

		fgets(buffer, sizeof(buffer), stdin);

		if(buffer[0] == 'y' || buffer[0] == 'Y') rerunServer = 1;
		else rerunServer = 0;

	}



	return 0;

}

void subServer(int cSocket, int player, int readPipe){

	int keepRunning = 1;
	int pDir[MAX_PLAYERS];

	while(keepRunning){
		read(readPipe, pDir, sizeof(pDir));
		write(cSocket, pDir, sizeof(pDir));

		keepRunning = 0;
		for(int x = 0; x < MAX_PLAYERS; x++){
			if(!(pDir[x] < 0)){
				keepRunning = 1;
			}
		}
	}
	close(cSocket);
	close(readPipe);

	exit(0);
}
