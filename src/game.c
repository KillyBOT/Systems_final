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

	int running = 1; //Whether the game is running or not

	SDL_Event e; // Event handler

	struct gameState g; //Game state
	struct gameCommand gC;

	int player; //Which player are you?
	int dir = 0;
	int prevDirection;

	char ip[64] = "";

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

	recv(readSock,&g,sizeof(g),MSG_WAITALL);
	drawGame(gRenderer, &g);

	prevDirection = g.pPos[player].dir;
	dir = prevDirection;

	for(int x = 3; x > 0; x--){
		printf("%d...\n", x);
		SDL_Delay(1000);
	}
	printf("GO!!!\n");

	while(running){

		//gC.player = player;
		//gC.cType = CMD_MOVE;

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

					case SDLK_DOWN:
					//changePlayerDir(g,0,DIR_DOWN);
					dir = DIR_DOWN;
					break;

					case SDLK_LEFT:
					//changePlayerDir(g,0,DIR_LEFT);
					dir = DIR_LEFT;
					break;

					case SDLK_RIGHT:
					//changePlayerDir(g,0,DIR_RIGHT);
					dir = DIR_RIGHT;
					break;

					default:
					dir = prevDirection;
					break;
				}
			}
		}

		prevDirection = dir;
		//printf("Dir: %d\n", dir);

		write(writeSock,&dir,sizeof(int));

		recv(readSock,&g,sizeof(struct gameState),MSG_WAITALL);

		drawGame(gRenderer,&g);

		if(g.pData[player] == PLAYER_STATE_JUSTDEAD) printf("You died!\n");

		running = 0;
		for(int x = 0; x < g.pNum; x++){
			if(g.pData[x] > 0 && g.pData[x] != PLAYER_STATE_DEAD){
				running = 1;
				break;
			}
		}
		//printf("%d\n", g.pLen[player]);

		SDL_Delay(TICSPEED);
		//printf("Test\n");

	}
		
	//deleteState(g);

	printLeaderBoard(&g);

	close(writeSock);
	close(readSock);
	closeSDL(gWindow,gRenderer);

	return 0;
}