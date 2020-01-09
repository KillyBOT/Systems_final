#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"
#include "network.h"

#define PIPE_WRITE_FD 5

void subServer(int cSocket, int readPipe);

int main(int argc, char* argv[]){

	int lSocket; //Listening socket
	int cSocket[MAX_PLAYERS]; //Client sockets for each client
	int subServer_count; //Number of sub serverss
	int pPipe[MAX_PLAYERS][2]; //Pipes for communicating to each player 
	struct gameCommand gC;
	struct gameCommand doNothing;
	//struct gameState* g = createState();
	int f;
	int pConnected = 0;
	int acceptConnects = 1;
	int runServer = 1;

	fd_set read_fds; //File descriptor to read from

	lSocket = server_setup();

	doNothing.cType = CMD_NOTHING;

	while(pConnected < MAX_PLAYERS && acceptConnects){

		cSocket[pConnected] = server_connect(lSocket);
		write(cSocket[pConnected],&pConnected,sizeof(pConnected));

		pipe(pPipe[pConnected]);
		printf("%d %d\n", pPipe[pConnected][0], pPipe[pConnected][1]);

		f = fork();
		if(f == 0){
			close(pPipe[pConnected][PIPE_WRITE]);
			subServer(cSocket[pConnected],pPipe[pConnected][PIPE_READ]); //This is the write port
		} else {
			close(cSocket[pConnected]);
			close(pPipe[pConnected][PIPE_READ]);
			cSocket[pConnected] = server_connect(lSocket);
			subServer_count++;
			pConnected++;

			acceptConnects = 0;
		}
	}

	while(runServer){

		fd_set client_fds;// The file descriptors for each client

		FD_ZERO(&client_fds);

		for(int n = 0; n < pConnected; n++){
			FD_SET(cSocket[n],&client_fds);
		}

		select(cSocket[pConnected - 1] + 1, &client_fds, NULL, NULL, NULL);

		//See which socket recieved data

		int currentClient;

		for(int n = 0; n < pConnected; n++){
			if(FD_ISSET(cSocket[n],&client_fds)){
				currentClient = n;
				break;
			}
		}

		printf("%d\n", currentClient);

		read(cSocket[currentClient],&gC,sizeof(gC));

		printf("Client %d says do command %d\n", currentClient, gC.cType);
		if(gC.cType == CMD_MOVE) printf("Dir: %d\n", gC.dir);

		doNothing.player = currentClient;

		for(int n = 0; n < pConnected; n++){
			write(pPipe[n][PIPE_WRITE],&gC,sizeof(gC));
		}

		//gC.cType = -1; //Close the sub server
		//write(pPipe[currentClient][PIPE_WRITE], &gC, sizeof(gC));
		
	}

	for(int n = 0; n < pConnected; n++){
		close(cSocket[n]);
		close(pPipe[n][PIPE_WRITE]);
	}

}

void subServer(int cSocket, int readPipe){

	struct gameCommand gC;

	while(1){
		read(readPipe, &gC, sizeof(gC));

		//printf("%d\n", gC.cType);

		if(gC.cType < 0){
			close(cSocket);
			close(readPipe);
			
			exit(0);
		}

		write(cSocket, &gC, sizeof(gC));

		/*close(cSocket);
		close(readPipe);
		
		exit(0);*/
	}
}