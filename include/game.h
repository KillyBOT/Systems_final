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
#define PLAYER_1_STARTDIR DIR_LEFT
#define PLAYER_2_STARTDIR DIR_RIGHT
#define PLAYER_3_STARTDIR DIR_DOWN
#define PLAYER_4_STARTDIR DIR_UP

#define CMD_MOVE 0
#define CMD_UPDATE 1
#define CMD_CHANGEPLAYER 2
#define CMD_ADDPLAYER 3
#define CMD_KILLPLAYER 4
#define CMD_NOTHING 5
#define CMD_PRINT 6

typedef unsigned char col; //Color

struct playerPos{
	int x;
	int y;
	int dir;
};

struct gameState{
	struct playerPos pPos[MAX_PLAYERS];
	int pData[MAX_PLAYERS];
	int lBoard[MAX_PLAYERS][2];
	int pLen[MAX_PLAYERS];

	int pNum;
	int pNewPlayer;
	int lBoardPos;
	int lBoardPlace;
	long tics;

	col board[MAPSIZE * MAPSIZE];
};

struct gameCommand{
	int cType; //The type of command

	int player; //Player to do the comamnd on
	int dir; //Direction to move
	
	char* msg; //Message to sends
};
