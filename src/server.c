#include "snakeEater.h"
#include "game.h"
#include "network.h"

void subServer(int cSocket);

int main(int argc, char* argv[]){

	int lSocket; //Listening socket
	int cSocket[MAX_PLAYERS]; //Client sockets for each 
	int subServer_count; //Number of sub serverss
	char buffer[512];
	struct gameCommand* gC = (struct gameCommand*)calloc(sizeof(struct gameCommand),1);

	fd_set read_fds; //File descriptor to read from

	lSocket = server_setup();

	while(1){

		FD_ZERO(&read_fds);
		FD_SET(STDIN_FILENO, &read_fds);
		FD_SET(lSocket, &read_fds);

		select(lSocket + 1, &read_fds, NULL, NULL, NULL);

		if(FD_ISSET(STDIN_FILENO, &read_fds)){
			fgets(buffer, sizeof(buffer),stdin);
			gC->cType = (int)strtol(buffer,NULL,10);
			printf("Current number of sub servers: %d\n", subServer_count);
		}

		if(FD_ISSET(lSocket, &read_fds)){

			int f;

			cSocket[0] = server_connect(lSocket);

			f = fork();
			if(!f){
				subServer(cSocket[0]);
			} else {
				subServer_count++;
				close(cSocket[0]);
			}
		}
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