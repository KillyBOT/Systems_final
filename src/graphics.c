#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"
#include "graphics.h"

int initSDL(SDL_Window* w, SDL_Renderer* r){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		return 1;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")){
		printf("Linear texture filtering could not be enabled!\n");
	}

	return 0;
}

void drawGame(SDL_Renderer* r, struct gameState* g){

	int rectSize = SCREEN_HEIGHT / MAPSIZE;
	int padding = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;

	SDL_SetRenderDrawColor(r, 0xB0, 0xB0, 0xB0, 0xFF);
	SDL_RenderClear(r);

	for(int y = 0; y < MAPSIZE; y++){
		for(int x = 0; x < MAPSIZE; x++){
			SDL_Rect currentRect = {(x * rectSize), y * rectSize, rectSize, rectSize};

			switch(getCol(g,x,y)){
				case 0:
				SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
				break;
				case PLAYER_1 + 1:
				SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);
				break;
				case PLAYER_2 + 1:
				SDL_SetRenderDrawColor(r, 0x00, 0x00, 0xFF, 0xFF);
				break;
				case PLAYER_3 + 1:
				SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0xFF);
				break;
				case PLAYER_4 + 1:
				SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0x00, 0xFF);
				break;
				case PLAYER_5 + 1:
				SDL_SetRenderDrawColor(r, 0xFF, 0x80, 0x00, 0xFF);
				break;
				case PLAYER_6 + 1:
				SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0xFF, 0xFF);
				break;
				case PLAYER_7 + 1:
				SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0xFF, 0xFF);
				break;
				case PLAYER_8 + 1:
				SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
				break;
				default:
				break;
			}

			SDL_RenderFillRect(r, &currentRect);
		}
	}

	for(int p = 0; p < MAX_PLAYERS; p++){
		if(g->pData[p] == PLAYER_STATE_ALIVE){
			SDL_Rect currentRect = {(g->pPos[p].x * rectSize), g->pPos[p].y * rectSize, rectSize, rectSize};

			switch(p){
				case PLAYER_1:
				SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);
				break;
				case PLAYER_2:
				SDL_SetRenderDrawColor(r, 0x00, 0x00, 0xFF, 0xFF);
				break;
				case PLAYER_3:
				SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0xFF);
				break;
				case PLAYER_4:
				SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0x00, 0xFF);
				break;
				case PLAYER_5:
				SDL_SetRenderDrawColor(r, 0xFF, 0x80, 0x00, 0xFF);
				break;
				case PLAYER_6:
				SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0xFF, 0xFF);
				break;
				case PLAYER_7:
				SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0xFF, 0xFF);
				break;
				case PLAYER_8:
				SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
				break;
				default:
				break;
			}

			SDL_RenderDrawRect(r, &currentRect);
		}

	}

	SDL_RenderPresent(r);
}

void closeSDL(SDL_Window* w, SDL_Renderer* r){
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	w = NULL;
	r = NULL;

	SDL_Quit();
}
