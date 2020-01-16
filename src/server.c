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
	//struct gameState* g = createState();
	//struct gameUpdate* u = malloc(sizeof(struct gameUpdate));
	int pDir[MAX_PLAYERS];
	int f;
	int pToAccept;
	int pConnected;
	int dir;

	int acceptConnects;
	int runServer;
	int rerunServer = 1;
	char buffer[4];

	if(argc > 1) pToAccept = atoi(argv[1]);
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
			//addPlayer2(g,pConnected);
			//u->data[pConnected] = PLAYER_STATE_ALIVE;
			pDir[pConnected] = PLAYER_STATE_ALIVE;

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

		//printf("%d %d %d %d\n", u->player1Dir,u->player2Dir,u->player3Dir,u->player4Dir);

		for(int n = 0; n < pConnected; n++){
			//printState(g);
			write(pPipe[n][PIPE_WRITE],pDir,sizeof(pDir));
		}

		//fd_set client_fds;// The file descriptors for each client

		while(runServer){
			for(int n = 0; n < pConnected; n++){
				recv(cSocket[n],&dir,sizeof(dir),MSG_WAITALL);

				//printf("Client %d says go in direction %d\n", n, dir);

				pDir[n] = dir;
			}

			// for(int x = 0; x < MAX_PLAYERS; x++){
			// 	printf("%d ",pDir[x]);
			// }
			// printf("\n");

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

	//struct gameUpdate* u = malloc(sizeof(struct gameUpdate));
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
	//free(u);

	exit(0);
}