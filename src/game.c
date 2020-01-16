#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"
#include "graphics.h"
#include "network.h"

int main(int argc, char* argv[]){

	int readSock; //Read from sub server
	int writeSock; //Write to main server

	SDL_Window* gWindow = NULL;// Window
	SDL_Renderer* gRenderer = NULL;// Renderer

	int running; //Whether the game is running or not
	int rerunGame = 1;
	char buffer[4];

	SDL_Event e; // Event handler

	struct gameState* g; //Game state
	//struct gameUpdate* u = malloc(sizeof(struct gameUpdate)); //Game update
	int pDir[MAX_PLAYERS];

	int player; //Which player are you?
	int pNum;
	int dir;
	int prevDirection;

	struct timeval stop, start;
	int latency; //How much time it took to do one tic

	while(rerunGame){
		char ip[64] = "";

		running = 1;
		dir = 0;
		g = createState();

		if(initSDL(gWindow, gRenderer)) {
			printf("Error initializing SDL! Closing...\n");
			return 1;
		}
		
		gWindow = SDL_CreateWindow("Snake Eater",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be initialized! Error: %s\n", SDL_GetError());
			return 1;
		}

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if(gRenderer == NULL){
			printf("Renderer could not be initialized! Error: %s\n", SDL_GetError());
			return 1;
		}

		if(argc > 1){
			strcpy(ip,argv[1]);
		} else {
			strcpy(ip,TEST_IP);
		}

		readSock = client_setup(ip);
		read(readSock, &player, sizeof(player));
		printf("You are player %d.\n", player);
		printf("Color: ");
		switch(player){
			case PLAYER_1:
			printf("RED");
			break;
			case PLAYER_2:
			printf("BLUE");
			break;
			case PLAYER_3:
			printf("GREEN");
			break;
			case PLAYER_4:
			printf("YELLOW");
			break;
			default:
			printf("ERROR! Tell Kyle if you see this!");
			break;
		}
		printf("\n");

		writeSock = client_setup(ip);

		printf("Waiting for other players to connect...\n");

		recv(readSock,pDir,sizeof(pDir),MSG_WAITALL);
		
		for(int p = 0; p < MAX_PLAYERS; p++){
			if(pDir[p] == PLAYER_STATE_ALIVE){
				addPlayer2(g,p);
			}
		}

		

		drawGame(gRenderer, g);

		prevDirection = g->pPos[player].dir;
		dir = prevDirection;

		printf("All players have connected.\n Starting in\n");

		for(int x = 3; x > 0; x--){
			printf("%d...\n", x);
			SDL_Delay(1000);
		}
		printf("\n");
		printf("GO!!!\n");

		while(running){

			//gC.player = player;
			//gC.cType = CMD_MOVE;

			gettimeofday(&start,NULL);

			while(SDL_PollEvent(&e) != 0){

				if( e.type == SDL_QUIT){
					running = 0;
				}
				else if( e.type == SDL_KEYDOWN){
					switch(e.key.keysym.sym){
						case SDLK_UP:
						//changePlayerDir(g,0,DIR_UP);
						dir = DIR_UP;
						break;

						case SDLK_w:
						dir = DIR_UP;
						break;

						case SDLK_DOWN:
						//changePlayerDir(g,0,DIR_DOWN);
						dir = DIR_DOWN;
						break;

						case SDLK_s:
						dir = DIR_DOWN;
						break;

						case SDLK_LEFT:
						//changePlayerDir(g,0,DIR_LEFT);
						dir = DIR_LEFT;
						break;

						case SDLK_a:
						dir = DIR_LEFT;
						break;

						case SDLK_RIGHT:
						//changePlayerDir(g,0,DIR_RIGHT);
						dir = DIR_RIGHT;
						break;

						case SDLK_d:
						dir = DIR_RIGHT;
						break;

						default:
						dir = prevDirection;
						break;
					}
				}
			}

			if(g->pData[player] == PLAYER_STATE_DEAD) dir = -1;

			prevDirection = dir;

			write(writeSock,&dir,sizeof(int));

			recv(readSock,pDir,sizeof(pDir),MSG_WAITALL);

			//printf("Dir: %d\n", dir);

			// int temp;
			// for(int p = 0; p < MAX_PLAYERS / 2; p++){ //Endian-ness is annoying
			// 	temp = u->data[p];
			// 	u->data[p] = u->data[MAX_PLAYERS - p - 1];
			// 	u->data[MAX_PLAYERS - p - 1] = temp;
			// }

			//printf("You: %d\n", player);
			for(int p = 0; p < MAX_PLAYERS; p++){
				//printf("Player: %d. Dir: %d\n", p, pDir[p]);
				changePlayerDir(g,p,pDir[p]);
			}

			updateState(g);

			drawGame(gRenderer,g);

			if(g->pData[player] == PLAYER_STATE_JUSTDEAD) printf("You died!\n");

			running = 0;
			for(int x = 0; x < g->pNum; x++){
				if(pDir[x] > 0){
					running = 1;
					break;
				}
			}

			gettimeofday(&stop, NULL);
			//printf("%d\n", g.pLen[player]);

			latency = ((stop.tv_usec - start.tv_usec) / 1000) + (stop.tv_sec - start.tv_sec) * 1000;
			printf("Latency: %dms\n", latency);

			if(latency > TICSPEED - 1) latency = TICSPEED - 1;
			SDL_Delay(TICSPEED - latency);
			//printf("Test\n");

		}
			
		printf("Game over!\n\n\n");
		printLeaderBoard(g);

		//free(u);
		deleteState(g);

		close(writeSock);
		close(readSock);
		closeSDL(gWindow,gRenderer);

		printf("\nDo you want to play again? [Y/N]\n");

		fgets(buffer,sizeof(buffer),stdin);

		if(buffer[0] == 'y' || buffer[0] == 'Y') {
			rerunGame = 1;
			printf("Reconnecting to server...\n");
		}
		else rerunGame = 0;
	}

	return 0;
}