#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"
#include "graphics.h"
#include "network.h"

int main(int argc, char* argv[]){

	int readSock; //Read from sub server
	int writeSock; //Write to main server
	struct gameCommand *gC; //Game command

	SDL_Window* gWindow = NULL;// Window
	SDL_Renderer* gRenderer = NULL;// Renderer

	int running = 1; //Whether the game is running or not

	SDL_Event e; // Event handler

	struct gameState *g; //Game state

	int player; //Which player are you?

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
	read(readSock, &player, sizeof(int));
	read(readSock, g, sizeof(g));
	gC = calloc(sizeof(struct gameCommand),1);

	//addPlayer(g);
	//addPlayer(g);
	//addPlayer(g);
	//addPlayer(g);

	while(running){

		while(SDL_PollEvent(&e) != 0){
			if( e.type == SDL_QUIT){
				running = 0;
			}
			else if( e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_UP:
					changePlayerDir(g,0,DIR_UP);
					break;

					case SDLK_DOWN:
					changePlayerDir(g,0,DIR_DOWN);
					break;

					case SDLK_LEFT:
					changePlayerDir(g,0,DIR_LEFT);
					break;

					case SDLK_RIGHT:
					changePlayerDir(g,0,DIR_RIGHT);
					break;

					default:
					break;
				}
			}
		}

		drawGame(gRenderer,g);
		// updateState(g);
		SDL_Delay(TICSPEED);

		//read(sSock, gC, sizeof(struct gameCommand));
		//printf("Server says do command %d\n", gC->cType);
		running = 0;

	}
		
	deleteState(g);
	free(gC);

	closeSDL(gWindow,gRenderer);

	return 0;
}