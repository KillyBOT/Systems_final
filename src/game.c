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

	struct gameState *g = createState(); //Game state
	struct gameCommand gC;

	int player; //Which player are you?
	int prevDirection;

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

	readSock = client_setup(TEST_IP);
	writeSock = client_setup(TEST_IP);
	read(readSock, &player, sizeof(player));

	//addPlayer(g);
	//addPlayer(g);
	//addPlayer(g);
	//addPlayer(g);
	printf("%d\n", player);

	addPlayer2(g,player);
	gC.cType = CMD_ADDPLAYER;
	gC.player = player;
	write(writeSock,&gC,sizeof(gC));
	prevDirection = g->pData[player];

	while(running){

		gC.cType = CMD_MOVE;
		gC.dir = prevDirection;

		while(SDL_PollEvent(&e) != 0){

			if( e.type == SDL_QUIT){
				running = 0;
			}
			else if( e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_UP:
					//changePlayerDir(g,0,DIR_UP);
					gC.dir = DIR_UP;
					break;

					case SDLK_DOWN:
					//changePlayerDir(g,0,DIR_DOWN);
					gC.dir = DIR_DOWN;
					break;

					case SDLK_LEFT:
					//changePlayerDir(g,0,DIR_LEFT);
					gC.dir = DIR_LEFT;
					break;

					case SDLK_RIGHT:
					//changePlayerDir(g,0,DIR_RIGHT);
					gC.dir = DIR_RIGHT;
					break;

					default:
					break;
				}
			}
		}

		prevDirection = gC.dir;

		write(writeSock,&gC,sizeof(gC));

		SDL_Delay(TICSPEED);


		read(readSock, &gC, sizeof(gC));

		//printf("Server says do command %d on player %d\n", gC.cType, gC.player);
		if(gC.cType == CMD_MOVE) printf("Dir: %d\n", gC.dir);

		process(g,gC);

		drawGame(gRenderer,g);
		// updateState(g);
	}
		
	deleteState(g);

	closeSDL(gWindow,gRenderer);

	return 0;
}