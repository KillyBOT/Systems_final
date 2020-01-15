#include "snakeEater.h"
#include "game.h"
#include "gameFuncs.h"

struct gameState* createState(){
	struct gameState* newState = malloc(sizeof(struct gameState));

	//newState->pPos = calloc(sizeof(struct playerPos), MAX_PLAYERS);

	//Set starting positions for each player
	newState->pPos[PLAYER_1].x = START_PADDING;
	newState->pPos[PLAYER_1].y = (MAPSIZE / 2);
	newState->pPos[PLAYER_1].dir = DIR_RIGHT;

	newState->pPos[PLAYER_2].x = MAPSIZE-1-START_PADDING;
	newState->pPos[PLAYER_2].y = (MAPSIZE / 2);
	newState->pPos[PLAYER_2].dir = DIR_LEFT;

	newState->pPos[PLAYER_3].x = (MAPSIZE / 2);
	newState->pPos[PLAYER_3].y = START_PADDING;
	newState->pPos[PLAYER_3].dir = DIR_DOWN;

	newState->pPos[PLAYER_4].x = (MAPSIZE / 2);
	newState->pPos[PLAYER_4].y = MAPSIZE-1-START_PADDING;
	newState->pPos[PLAYER_4].dir = DIR_UP;

	//newState->pData = calloc(sizeof(int), MAX_PLAYERS);
	newState->lBoardPos = 0;
	newState->lBoardPlace = 0;

	for(int x = 0; x < MAX_PLAYERS; x++){
		newState->pData[x] = 0;
		newState->pLen[x] = 0;
		newState->lBoard[x][0] = 0;
		newState->lBoard[x][1] = 0;
	}

	newState->pNewPlayer = 0;
	newState->tics = 0;

	//newState->board = calloc(sizeof(col), MAPSIZE * MAPSIZE);
	for(int y = 0; y < MAPSIZE; y++){
		for(int x = 0; x < MAPSIZE; x++){
			newState->board[(y * MAPSIZE) + x] = 0;
		}
	}

	return newState;

}

void addPlayer(struct gameState* g){
	if(g->pNewPlayer < MAX_PLAYERS){
		g->pData[g->pNewPlayer] = PLAYER_STATE_ALIVE;
		g->pNewPlayer++;
		g->lBoardPlace++;
		g->pNum++;
		g->tics++;
	}

}

void addPlayer2(struct gameState *g, int player){
	if(player < MAX_PLAYERS){
		g->pData[player] = PLAYER_STATE_ALIVE;
		g->lBoardPlace++;
		g->pNum++;
		g->tics++;
	}
}

void changePlayerDir(struct gameState* g, int player, int newDir){
	if(player < MAX_PLAYERS && g->pData[player] > 0){
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
	int lBoardPlaceToAdd = 0;

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

			//printf("%d %d\n", newX, newY);
			if(checkDeath(g,cPlayer,newX,newY)) g->pData[cPlayer] = PLAYER_STATE_JUSTDEAD;
			else{
				g->pPos[cPlayer].x = newX;
				g->pPos[cPlayer].y = newY;
				g->pLen[cPlayer] += 1;
			}

		} else if(g->pData[cPlayer] == PLAYER_STATE_JUSTDEAD){
			printf("Player %d just died!\n", cPlayer);
			g->pData[cPlayer] = PLAYER_STATE_DEAD;
			g->lBoard[g->lBoardPos][0] = cPlayer;
			g->lBoard[g->lBoardPos][1] = g->lBoardPlace;
			g->lBoardPos++;
			lBoardPlaceToAdd++;
		}

	}
	g->lBoardPlace -= lBoardPlaceToAdd;
	//printf("%d\n",g->lBoardPlace);
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

void movePlayer(struct gameState* g, int player, int dir){
	changePlayerDir(g,player,dir);
	updatePlayerState(g,player);
}

void deleteState(struct gameState* g){
	//free(g->pPos);
	//free(g->pData);
	//free(g->board);
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

// void process(struct gameState* g, struct gameCommand gC){
// 	switch(gC.cType){

// 		case CMD_ADDPLAYER:
// 		addPlayer2(g, gC.player);
// 		break;

// 		case CMD_MOVE:
// 		movePlayer(g,gC.player,gC.dir);
// 		break;

// 		case CMD_NOTHING:
// 		break;

// 		default:
// 		printf("Unknown command!\n");
// 		break;
// 	}
// }

col getCol(struct gameState* g, int x, int y){
	return g->board[ (x * MAPSIZE) + y];
}
col setCol(struct gameState* g, int x, int y, col newColor){
	col oldCol = getCol(g, x, y);
	g->board[(x * MAPSIZE) + y] = newColor;
	return oldCol;
}

col getAttr(col attr, int player){
	col toRet = 3;
	toRet <<= player * 2;
	toRet &= attr;
	toRet >>= player * 2;
	return toRet;
}

col setAttr(col oldAttr, col newVal, int player){
	col finalAttr = 255;
	finalAttr ^= (3 << (player * 2));
	finalAttr = oldAttr & finalAttr;
	col newAttr = newVal & 3;
	newAttr <<= player * 2;
	finalAttr |= newAttr;
	return finalAttr;
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

void printLeaderBoard(struct gameState* g){
	printf("Leaderboard: [Place] [Player] [Length]\n");
	for(int x = g->lBoardPos - 1; x >= 0; x--){
		printf("             %-8d%-9d%d\n", g->lBoard[x][1], g->lBoard[x][0], g->pLen[g->lBoard[x][0]]);
	}
}

// struct gameCommand createCommand(int cType, int player){
// 	struct gameCommand gC;
// 	gC.cType = cType;
// 	gC.player = player;

// 	return gC;
// }

// struct gameCommand getCommand(int fd){
// 	struct gameCommand gC;

// 	read(fd, &gC.cType, sizeof(int));
// 	read(fd, &gC.player, sizeof(int));

// 	switch(gC.cType){
// 		default:
// 		printf("Unknown command!\n");
// 		break;
// 	}

// 	return gC;
// }