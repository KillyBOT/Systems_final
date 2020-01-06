#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int initSDL(SDL_Window* w, SDL_Renderer* r);
void drawGame(SDL_Renderer* r, struct gameState* g);
void closeSDL(SDL_Window* w, SDL_Renderer* r);