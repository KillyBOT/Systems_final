#define MAPSIZE 150
#define TICSPEED 35
#define MSG_MAXSIZE 512

#define MAX_PLAYERS 4
#define PLAYER_1 0
#define PLAYER_2 1
#define PLAYER_3 2
#define PLAYER_4 3

#define PLAYER_STATE_NA 0
#define PLAYER_STATE_ALIVE 1
#define PLAYER_STATE_JUSTDEAD 2
#define PLAYER_STATE_DEAD 3

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define START_PADDING 5 //How far away from the edge you are when you start

#define CMD_MOVE 0
#define CMD_UPDATE 1
#define CMD_ADDPLAYER 2
#define CMD_KILLPLAYER 3
#define CMD_NOTHING 4
#define CMD_PRINT 5

typedef unsigned char col; //Color

struct playerPos{
	int x;
	int y;
	int dir;
};

struct gameState{
	struct playerPos* pPos;
	int* pData;

	int pNewPlayer;
	long tics;

	col* board;
};

struct gameCommand{
	int cType; //The type of command

	int player; //Player to do the comamnd on
	int dir; //Direction to move
	
	char* msg; //Message to sends
};
