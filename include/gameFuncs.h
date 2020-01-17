col getCol(struct gameState* g, int x, int y);
col setCol(struct gameState* g, int x, int y, col newColor);
int checkDeath(struct gameState* g, int player, int x, int y);

struct gameState* createState();
void addPlayer(struct gameState* g);
void addPlayer2(struct gameState* g, int player);
void changePlayerDir(struct gameState* g, int player, int newDir);
void updateState(struct gameState* g);
void updatePlayerState(struct gameState* g, int player);
void movePlayer(struct gameState* g, int player, int dir);
void deleteState(struct gameState* g);

void printState(struct gameState* g);
void printLeaderBoard(struct gameState* g);
