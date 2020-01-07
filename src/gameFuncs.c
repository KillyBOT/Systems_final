#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"

struct gameState* createState(){
	struct gameState* newState = malloc(sizeof(struct gameState));

	newState->pPos = calloc(sizeof(struct playerPos), MAX_PLAYERS);

	//Set starting positions for each player
	newState->pPos[0].x = START_PADDING;
	newState->pPos[0].y = (MAPSIZE / 2);
	newState->pPos[0].dir = DIR_RIGHT;

	newState->pPos[1].x = MAPSIZE-1-START_PADDING;
	newState->pPos[1].y = (MAPSIZE / 2);
	newState->pPos[1].dir = DIR_LEFT;

	newState->pPos[2].x = (MAPSIZE / 2);
	newState->pPos[2].y = START_PADDING;
	newState->pPos[2].dir = DIR_DOWN;

	newState->pPos[3].x = (MAPSIZE / 2);
	newState->pPos[3].y = MAPSIZE-1-START_PADDING;
	newState->pPos[3].dir = DIR_UP;

	newState->pData = calloc(sizeof(int), MAX_PLAYERS);

	newState->pNewPlayer = 0;
	newState->tics = 0;

	newState->board = calloc(sizeof(col), MAPSIZE * MAPSIZE);

	return newState;

}

void addPlayer(struct gameState* g){
	if(g->pNewPlayer < MAX_PLAYERS){
		g->pData[g->pNewPlayer] = PLAYER_STATE_ALIVE;
		g->pNewPlayer++;
	}

}

void changePlayerDir(struct gameState* g, int player, int newDir){
	if(player < MAX_PLAYERS){
		int ifSwitch = 1;
		switch(newDir){
			case DIR_UP:
			if(g->pPos[player].dir == DIR_DOWN) ifSwitch = 0;
			break;

			case DIR_DOWN:
			if(g->pPos[player].dir == DIR_UP) ifSwitch = 0;
			break;

			case DIR_LEFT:
			if(g->pPos[player].dir == DIR_RIGHT) ifSwitch = 0;
			break;

			case DIR_RIGHT:
			if(g->pPos[player].dir == DIR_LEFT) ifSwitch = 0;
			break;

			default:
			ifSwitch = 1;
			break;
		}

		if(ifSwitch) g->pPos[player].dir = newDir;
	}
}

void updateState(struct gameState* g){

	int newX, newY, dir;

	for(int cPlayer = 0; cPlayer < MAX_PLAYERS; cPlayer++){
		if(g->pData[cPlayer] == PLAYER_STATE_ALIVE){ //If the player is alive
			newX = g->pPos[cPlayer].x;
			newY = g->pPos[cPlayer].y;
			dir = g->pPos[cPlayer].dir;

			setCol(g, g->pPos[cPlayer].x, g->pPos[cPlayer].y, (col)cPlayer+1);

			if(dir == DIR_UP) newY--;
			if(dir == DIR_DOWN) newY++;
			if(dir == DIR_LEFT) newX--;
			if(dir == DIR_RIGHT) newX++;

			if(checkDeath(g,cPlayer,newX,newY)) g->pData[cPlayer] = PLAYER_STATE_JUSTDEAD;
			else{
				g->pPos[cPlayer].x = newX;
				g->pPos[cPlayer].y = newY;
			}

		} else if(g->pData[cPlayer] == PLAYER_STATE_JUSTDEAD){
			g->pData[cPlayer] = PLAYER_STATE_DEAD;
		}

	}
	g->tics++;
}

void updatePlayerState(struct gameState* g, int player){

	int newX, newY, dir;

	if(g->pData[player] == PLAYER_STATE_ALIVE){ //If the player is alive
			newX = g->pPos[player].x;
			newY = g->pPos[player].y;
			dir = g->pPos[player].dir;

			setCol(g, g->pPos[player].x, g->pPos[player].y, (col)player+1);

			if(dir == DIR_UP) newY--;
			if(dir == DIR_DOWN) newY++;
			if(dir == DIR_LEFT) newX--;
			if(dir == DIR_RIGHT) newX++;

			if(checkDeath(g,player,newX,newY)) g->pData[player] = PLAYER_STATE_JUSTDEAD;
			else{
				g->pPos[player].x = newX;
				g->pPos[player].y = newY;
			}

		} else if(g->pData[player] == PLAYER_STATE_JUSTDEAD){
			g->pData[player] = PLAYER_STATE_DEAD;
		}
}

void deleteState(struct gameState* g){
	free(g->pPos);
	free(g->pData);
	free(g->board);
	free(g);
}

int checkDeath(struct gameState* g, int player, int x, int y){

	if(getCol(g,x,y)) return 1;

	if(x < 0 || y < 0 || x >= MAPSIZE || y >= MAPSIZE) return 1;

	for(int oPlayer = 0; oPlayer < MAX_PLAYERS; oPlayer++){
		if(oPlayer != player && (g->pData[oPlayer] == PLAYER_STATE_ALIVE || g->pData[oPlayer] == PLAYER_STATE_JUSTDEAD)) {
			if(g->pPos[oPlayer].x == x && g->pPos[oPlayer].y == y){
				g->pData[oPlayer] = PLAYER_STATE_JUSTDEAD;
				setCol(g,x,y,(col)player);
				return 1;
			}
		}
	}

	return 0;
}

col getCol(struct gameState* g, int x, int y){
	return g->board[ (x * MAPSIZE) + y];
}
col setCol(struct gameState* g, int x, int y, col newColor){
	col oldCol = getCol(g, x, y);
	g->board[(x * MAPSIZE) + y] = newColor;
	return oldCol;
}

void printState(struct gameState* g){

	printf("Tics: %ld\n", g->tics);
	printf("Player states: ");
	for(int n = 0; n < MAX_PLAYERS; n++){
		printf("%d ",g->pData[n]);
	}
	printf("\n");

	for(int y = 0; y < MAPSIZE; y++){
		for(int x = 0; x < MAPSIZE; x++){

			col toPrint = 0;
			for(int p = 0; p < MAX_PLAYERS; p++){
				if(g->pData[p] == PLAYER_STATE_ALIVE && g->pPos[p].x == x && g->pPos[p].y == y) toPrint += 5 + p;
			}

			toPrint+=getCol(g,x,y);
			printf("%d ",(int)toPrint);
		}
		printf("\n");
	}
	printf("\n");
}