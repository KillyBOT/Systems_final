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
	printf("%d\n", player);

	gC.cType = CMD_ADDPLAYER;
	gC.player = player;
	write(writeSock,&gC,sizeof(gC));
	read(readSock,g,sizeof(g));

	/*switch(player){
		case PLAYER_1:
		prevDirection = PLAYER_1_STARTDIR;
		break;
		case PLAYER_2:
		prevDirection = PLAYER_2_STARTDIR;
		break;
		case PLAYER_3:
		prevDirection = PLAYER_3_STARTDIR;
		break;
		case PLAYER_$:
		prevDirection = PLAYER_4_STARTDIR;
		break;
		default:
		break;
	}*/

	prevDirection = g->pPos[player].dir;
	printf("%d\n", prevDirection);

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
					//gC.dir = DIR_UP;
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
					gC.dir = prevDirection;
					break;
				}
			}
		}

		prevDirection = gC.dir;

		write(writeSock,&gC,sizeof(gC));
		read(readSock,g,sizeof(g));
		printf("%d\n", g->pData[player]);

		//printf("Server says do command %d on player %d\n", gC.cType, gC.player);
		//if(gC.cType == CMD_MOVE) printf("Dir: %d\n", gC.dir);

		//process(g,gC);

		drawGame(gRenderer,g);
		// updateState(g);
		SDL_Delay(1000);
		printf("Test\n");

	}
		
	deleteState(g);

	closeSDL(gWindow,gRenderer);

	return 0;
}