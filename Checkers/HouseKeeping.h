/*

Header file containing all fucntion prototypes and define statements as well as all other include statements

Name: Noah Hinderle
Email: noah.hinderle@gmail.com
Date: July 7, 2021

*/

#include <string>
#include <iostream>
#include "stdlib.h"
#include <fstream>
#include "SDL.h"
#include "GamePiece.h"
#include "Player.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

// Defines the size of each of the positions on the board, 100px
#define UNIT 100
// Defines the FPS used for loop delays (ex. SDL_Delay(1000/FPS) will run the loop FPS times per second)
#define FPS 60
// Defines the size in pixels of each piece on the game board
#define PIECESIZE 50
// Defines the upper limit for any x coordinate on the board
#define maxX 7
// Defines the upper limit for any y coordinate on the board
#define maxY 7
// Defines the offset for the rectangle surrounding each game piece
#define PIECEOFFSET (UNIT - PIECESIZE) / 2

int closeConfirmation();
void drawBoard(SDL_Renderer*);
int drawMenu();
void initBoard(SDL_Renderer*, GamePiece**, SDL_Rect*, Player* p0, Player* p1);
SDL_Rect drawPiece(SDL_Renderer*, const char*, int, int);
void redrawPiece(SDL_Renderer*, GamePiece*, SDL_Rect*, int, int);
void initBoardPositions(GamePiece**, int**);
void freeBoard(int**);
void movePieceOnBoard(GamePiece*, int**, int, int);
int getSelectedPiece(SDL_Rect*, int, int);
void addText(SDL_Renderer*, TTF_Font*, const char*, SDL_Color, int, int, SDL_Rect*);
int checkRectSelected(SDL_Rect*, int, int);
int handlePieceSelected(SDL_Renderer*, int, GamePiece*, int**, SDL_Rect*);
void setRectCoords(SDL_Renderer*, int, int, SDL_Rect*);
void clearGreenRects(SDL_Renderer*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*);
