/*

Name: Noah Hinderle
Email: noah.hinderle@gmail.com
Date: July 16, 2021

This program is a game of checkers. The user can choose to play alone (against AI) or with another person on the same machine.
There will be an option to load the most recent game/some number of recent games to resume.

*/

#include "HouseKeeping.h"
#include "Helper.h"

int main(int argc, char *argv[]){
	
	int imgFlags = IMG_INIT_PNG;
	int close = 0, movedPiece = 0;
	int** gameBoard = (int**) malloc(sizeof(int*) * 8);
	int selectedPiece, selectedTeam, teamTurn = 0;
	std::string str;

	GamePiece* pieceList[24];

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		std::cout << "Error Initialising SDL" << std::endl;
		tearDown(0, gameBoard, NULL, NULL);
		return -1;

	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {

		std::cout << "Error Initialising SDL" << std::endl;
		tearDown(1, gameBoard, NULL, NULL);
		return -1;

	}

	if (TTF_Init() < 0) {

		std::cout << "Error Initialising SDL_TTF" << std::endl;
		tearDown(2, gameBoard, NULL, NULL);
		return -1;

	}

	TTF_Font* font = TTF_OpenFont("Raleway-Thin.ttf", 20);
	int drawRet = drawMenu();
	Player* p0 = new Player(0);
	Player* p1 = new Player(1);

	if (drawRet == -1) {

		std::cout << "An error occurred, exiting..." << std::endl;
		tearDown(3, gameBoard, p0, p1);
		return -1;

	} else if (drawRet == 1) {

		tearDown(3, gameBoard, p0, p1);
		return 0;

	} else if (drawRet == 2) {

		if (fexists(SAVEFILE)) {

			tearDown(-1, NULL, p0, p1);
			p0 = new Player;
			p1 = new Player;
			int loadCode = loadGame(p0, p1);
			if (loadCode == -1) {

				std::cout << "Error encountered while loading game, exiting..." << std::endl;
				tearDown(3, gameBoard, p0, p1);
				return -1;

			}

		} else {

			createModal("Load Failed", "Program failed to load saved game file, new game started", "Ok", NULL);

		}
		
		teamTurn = p0->getIsTurn() ? 0 : 1;

	}

	SDL_Window* window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAMESCREENWIDTH, GAMESCREENHEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;
	SDL_Rect rectList[24];
	SDL_Color blackColor = { 0, 0, 0 }, blueColor = { 0, 0, 255 }, redColor = { 255, 0, 0 };
	SDL_Rect* quitRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	SDL_SetRenderDrawColor(renderer, 200, 211, 201, 255);
	SDL_RenderClear(renderer);

	drawBoard(renderer);
	initBoard(renderer, pieceList, rectList, p0, p1);
	initBoardPositions(pieceList, gameBoard);
	addText(renderer, font, "Save & Quit Game", blackColor, 0, 0, quitRect);

	updateScore(renderer, p0, p1);

	if (!quitRect) {

		close = 1;

	}

	SDL_RenderPresent(renderer);

	while (close != 1) {

		SDL_PollEvent(&event);
		if (event.type == SDL_MOUSEBUTTONDOWN) {

			selectedPiece = getSelectedPiece(rectList, event.button.x, event.button.y);

			if (checkRectSelected(quitRect, event.button.x, event.button.y) == 0) {

				close = closeConfirmation();

				if (close == 1) {

					saveGame(p0, p1);

				}

			} else if (selectedPiece >= 0) {

				selectedTeam = pieceList[selectedPiece]->getTeam();
				if (selectedTeam == teamTurn) {

					movedPiece = selectedPiece;

					do {

						selectedTeam = pieceList[movedPiece]->getTeam();
						if (selectedTeam == teamTurn) {

							movedPiece = handlePieceSelected(renderer, movedPiece, pieceList[movedPiece], gameBoard, rectList, p0, p1);

						} else {

							break;

						}
						

					} while (movedPiece >= 0);

					close = movedPiece == -2 ? 1 : 0;
					if (movedPiece == -1) {

						teamTurn = !teamTurn;
						p0->setIsTurn(!p0->getIsTurn());
						p1->setIsTurn(!p1->getIsTurn());

					}

					selectedPiece = -1;

				}

			}

		}

		if (event.type == SDL_QUIT) {

			close = 1;

		}

		SDL_Delay(1000 / FPS);

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	tearDown(3, gameBoard, p0, p1);

	return 0;

}

/*

This function checks if the given file exists

@param const char* fname: the name of the file to be checked for existance

@return bool: returns true if file exists, false otherwise

*/
bool fexists(const char* fname) {

	return static_cast<bool>(std::ifstream(fname));

}

/*

This function saves the current game to the previous game file

@param Player* p0: player object to be written to file
@param Player* p1: player object to be written to file 

*/
void saveGame(Player* p0, Player* p1) {

	std::ofstream file(SAVEFILE);
	boost::archive::binary_oarchive outputArc(file);

	outputArc& BOOST_SERIALIZATION_NVP(*p0);
	outputArc& BOOST_SERIALIZATION_NVP(*p1);

	file.close();

}

/*

This function loads a game from the previous game file

@param Player* p0: player object that will have values loaded from file
@param Player* p1: player object that will have values loaded from file

@return int: returns 0 on successful read, returns -1 on failure to read file

*/
int loadGame(Player* p0, Player* p1) {

	if (fexists(SAVEFILE)) {

		std::ifstream file(SAVEFILE);

		boost::archive::binary_iarchive inputArc(file);

		inputArc& BOOST_SERIALIZATION_NVP(*p0);
		inputArc& BOOST_SERIALIZATION_NVP(*p1);

		file.close();
		return 0;

	}

	return -1;

}

/*

This function generates a popup window to ensure that the user wants to quit the current game

@return int: number indicating whether the user is choosing to exit the game

*/
int closeConfirmation() {

	return createModal("Quit Game", "Are you sure you want to save and quit?", "Yes", "No");

}

/*

This function draws the main menu for the game, allows users to start a new game, load previous, etc.

@return int: number indicating whether the menu was drawn successfully

*/
int drawMenu() {

	SDL_Window* window = SDL_CreateWindow("Main Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
	int close = 0, x, y;
	SDL_Rect* newRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	SDL_Rect* loadRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	SDL_Color blackText = { 0, 0, 0 };
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	TTF_Font* font = TTF_OpenFont("Raleway-Thin.ttf", 50);
	if (!font) {

		std::cout << "Failed to load file" << std::endl;
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		free(newRect);
		free(loadRect);
		return -1;

	}

	TTF_Font* font2 = TTF_OpenFont("Raleway-Thin.ttf", 30);
	if (!font2) {

		std::cout << "Failed to load file" << std::endl;
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		free(newRect);
		free(loadRect);
		return -1;

	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	addText(renderer, font, "WELCOME", blackText, 0, 0, newRect);
	addText(renderer, font2, "New Game", blackText, 0, 100, newRect);
	addText(renderer, font2, "Load Game", blackText, 0, 200, loadRect);
	SDL_RenderPresent(renderer);

	if (!newRect || !loadRect) {

		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		return -1;

	}
	
	while (!close) {

		SDL_Event event;
		SDL_PollEvent(&event);
		if (event.type == SDL_MOUSEBUTTONDOWN) {

			x = event.button.x;
			y = event.button.y;

			if (checkRectSelected(newRect, x, y) == 0) {

				close = 1;

			} else if (checkRectSelected(loadRect, x, y) == 0) {

				SDL_DestroyWindow(window);
				SDL_DestroyRenderer(renderer);
				free(newRect);
				free(loadRect);
				return 2;

			}

		}

		if (event.type == SDL_QUIT) {

			return 1;

		}

		SDL_Delay(1000 / FPS);

	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	free(newRect);
	free(loadRect);

	return 0;

}

/*

This function draws the game board to the window created in main()

@param SDL_Renderer* renderer: A pointer to the renderer used to draw the game board

*/
void drawBoard(SDL_Renderer* renderer) {

	SDL_Rect board, nextSquare;
	int currOffset = 2, currRectOffset = 1, order = 0;
	
	board.x = 100;
	board.y = 100;
	board.w = 800;
	board.h = 800;

	nextSquare.w = UNIT;
	nextSquare.h = UNIT;

	for (int i = 0; i < 8; i++) {
		
		nextSquare.x = (currOffset - 1) * UNIT;

		for (int i = 0; i < 8; i++) {

			if (i % 2 == order) {

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			} else {

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			}
			
			nextSquare.y = currRectOffset * UNIT;
			currRectOffset++;

			SDL_RenderFillRect(renderer, &nextSquare);
			SDL_RenderDrawRect(renderer, &nextSquare);

		}

		order = order == 1 ? 0 : 1;
		currRectOffset = 1;
		currOffset++;

	}

	SDL_SetRenderDrawColor(renderer, 120, 69, 30, 255);
	SDL_RenderDrawRect(renderer, &board);

}

/*

This function draws all of the game pieces originally

@param SDL_Renderer* renderer: renderer object to draw the checkers
@param GamePiece* pieceList[]: list of all pieces in the game
@param SDL_Rect rectList[]: list of all rectangles surrounding the pieces
@param Player* p0: the first player in the game
@param Player* p1: the second player in the game

*/
void initBoard(SDL_Renderer* renderer, GamePiece** pieceList, SDL_Rect* rectList, Player* p0, Player* p1) {

	GamePiece** pieces1 = p0->getPieces();
	GamePiece** pieces2 = p1->getPieces();
	SDL_Rect tempRect;
	tempRect.x = tempRect.y = -1;
	tempRect.w = tempRect.h = 0;
	int index = 0;
	std::copy(pieces1, pieces1 + 12, pieceList);
	std::copy(pieces2, pieces2 + 12, pieceList + 12);

	for (int i = 0; i < 12; i++) {

		if (pieces1[i]->getInPlay()) {

			rectList[index] = drawPiece(renderer, pieces1[i]->getIsKing() ? "redKingChecker.png" : "redChecker.png", ((pieces1[i]->getY() + 1) * UNIT) + PIECEOFFSET, ((pieces1[i]->getX() + 1) * UNIT) + PIECEOFFSET);

		} else {

			rectList[index] = tempRect;

		}

		index++;
		
	}

	for (int i = 0; i < 12; i++) {

		if (pieces2[i]->getInPlay()) {

			rectList[index] = drawPiece(renderer, pieces2[i]->getIsKing() ? "blueKingChecker.png" : "blueChecker.png", ((pieces2[i]->getY() + 1) * UNIT) + PIECEOFFSET, ((pieces2[i]->getX() + 1) * UNIT) + PIECEOFFSET);

		} else {

			rectList[index] = tempRect;

		}

		index++;

	}

	SDL_RenderPresent(renderer);

}

/*

This function draws a game piece based on the color chosen and coordinates supplied

@param SDL_Renderer* renderer: renderer object to draw the piece
@param const char* imageTitle: name of image file to be opened and used
@param int x: x coordinate of image on the board
@param int y: y coordinate of image on the board

@return SDL_Rect: rectangle object surrounding image drawn

*/
SDL_Rect drawPiece(SDL_Renderer* renderer, const char* imageTitle, int x, int y) {

	SDL_Surface* image = IMG_Load(imageTitle);
	SDL_Rect tempRect;
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = PIECESIZE;
	tempRect.h = PIECESIZE;

	if (!image) {

		std::cout << "IMG load failed, exiting... " << SDL_GetError() << std::endl;
		return tempRect;

	}

	SDL_Texture* texImg = SDL_CreateTextureFromSurface(renderer, image);
	SDL_RenderCopy(renderer, texImg, NULL, &tempRect);

	SDL_DestroyTexture(texImg);

	return tempRect;

}

/*

This function is used to redraw pieces on the board

@param SDL_Renderer* renderer: renderer object to draw the new piece with
@param GamePiece* piece: game piece object that is being redrawn
@param SDL_Rect* rect: rectanlge that surrounds the game piece
@param int prevX: x coordinate of the old location of the game piece
@param int prevY: y coordinate of the old location of the game piece

*/
void redrawPiece(SDL_Renderer* renderer, GamePiece* piece, SDL_Rect* rect, int prevX, int prevY) {

	SDL_Surface* image;

	if (piece->getIsKing()) {

		image = IMG_Load(piece->getTeam() == 0 ? "redKingChecker.png" : "blueKingChecker.png");

	} else {

		image = IMG_Load(piece->getTeam() == 0 ? "redChecker.png" : "blueChecker.png");

	}

	SDL_Rect tempRect;
	tempRect.x = ((prevY + 1) * UNIT);
	tempRect.y = ((prevX + 1) * UNIT);
	tempRect.w = UNIT;
	tempRect.h = UNIT;

	rect->x = ((piece->getY() + 1) * UNIT) + PIECEOFFSET;
	rect->y = ((piece->getX() + 1) * UNIT) + PIECEOFFSET;
	rect->w = PIECESIZE;
	rect->h = PIECESIZE;

	if (!image) {

		std::cout << "IMG load failed, exiting... " << SDL_GetError() << std::endl;
		return;

	}

	SDL_Texture* texImg = SDL_CreateTextureFromSurface(renderer, image);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &tempRect);
	SDL_RenderDrawRect(renderer, &tempRect);

	SDL_RenderCopy(renderer, texImg, NULL, rect);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(texImg);

}

/*

This function checks which rectangle has been selected in order to select the corresponding piece

@param SDL_Rect* rectList: list of rectangles surrounding each of the game pieces
@param int x: the x coordinate of the mouse when clicked
@param int y: the y coordinate of the mouse when clicked

@return int: the index of the rectangle that was selected

*/
int getSelectedPiece(SDL_Rect* rectList, int x, int y) {

	for (int i = 0; i < 24; i++) {

		if (rectList[i].x == -1 || rectList[i].y == -1 || rectList[i].h == 0 || rectList[i].w == 0) {

			continue;

		} else if ((x > rectList[i].x) && (x < (rectList[i].x + rectList[i].w)) && (y > rectList[i].y) && (y < (rectList[i].y + rectList[i].h))) {

			return i;

		}

	}

	return -3;

}

/*

This function initializes the 2D array containing all pieces on the board

@param GamePiece** pieceList: list of pieces on the board to be written into the array
@param int** gameBoard: 2D array representing all positions on the board and whether they are filled


*/
void initBoardPositions(GamePiece** pieceList, int** gameBoard) {

	for (int i = 0; i < 8; i++) {

		gameBoard[i] = (int*) calloc(8, sizeof(int));

	}

	if (!gameBoard) {

		return;

	}

	for (int i = 0; i < 24; i++) {

		if (pieceList[i]->getInPlay() && gameBoard[pieceList[i]->getX()]) {

			gameBoard[pieceList[i]->getX()][pieceList[i]->getY()] = 1;

		}

	}

}

/*

This function checks whether or not the passed rectangle object has been selected

@param SDL_Rect* rect: rectangle object to be checked for selection
@param int x: x coordinate of mouse at time of selection
@param int y: y coordinate of mouse at time of selection

@return int: returns 0 to represent the rectangle was selected and -1 to represent the rectangle was not selected

*/
int checkRectSelected(SDL_Rect* rect, int x, int y) {

	if ((x > rect->x) && (x < (rect->x + rect->w)) && (y > rect->y) && (y < (rect->y + rect->h))) {

		return 0;

	}

	return -1;

}

void removePiece(SDL_Renderer* renderer, int** gameBoard, GamePiece* piece, SDL_Rect* rectList) {

	int x = piece->getX(), y = piece->getY();
	SDL_Rect tempRect;
	tempRect.x = (y + 1) * UNIT;
	tempRect.y = (x + 1) * UNIT;
	tempRect.w = tempRect.h = UNIT;
	rectList[piece->getIndex()].w = 0;
	rectList[piece->getIndex()].h = 0;
	gameBoard[x][y] = 0;
	piece->setInPlay(false);

	SDL_RenderFillRect(renderer, &tempRect);
	SDL_RenderDrawRect(renderer, &tempRect);

}

/*

This function handles the user clicking on a specific game piece by drawing rectangles where the piece can be moved

@param SDL_Renderer* renderer: renderer object to draw rectangles to window
@param GamePiece* piece: the game piece that was selected by the user
@param int ** board: the game board tracking what positions are filled
@param SDL_Rect* rectList: list of rectangles that contain each other piece

@return int: returns the index of another piece that was selected or -1 for no other piece selected or -2 for close window

*/
int handlePieceSelected(SDL_Renderer* renderer, int index, GamePiece* piece, int** board, SDL_Rect* rectList, Player* p0, Player* p1) {

	int pieceX = piece->getX();
	int pieceY = piece->getY();
	int pieceTeam = piece->getTeam();
	int lessX = 0, lessY = 0, close = 0, jumpUL = 1, jumpUR = 1, jumpDL = 1, jumpDR = 1;
	SDL_Rect upLeft, upRight, downLeft, downRight;
	SDL_Event event;
	GamePiece* jumpedPieceUL, *jumpedPieceUR, *jumpedPieceDL, *jumpedPieceDR;
	jumpedPieceUL = jumpedPieceUR = jumpedPieceDL = jumpedPieceDR = NULL;
	upLeft.w = upRight.w = downLeft.w = downRight.w = UNIT;
	upLeft.h = upRight.h = downLeft.h = downRight.h = UNIT;
	upLeft.x = upRight.x = downLeft.x = downRight.x = -10;
	upLeft.y = upRight.y = downLeft.y = downRight.y = -10;

	std::cout << "PIECE X = " << pieceX << ", Y = " << pieceY << std::endl;

	SDL_SetRenderDrawColor(renderer, 51, 255, 51, 255);

	if (piece->getTeam() == 0 || piece->getIsKing()) {

		if (pieceX - 1 >= 0) {

			if (pieceY - 1 >= 0) {

				if (board[pieceX - 1][pieceY - 1] == 0) {

					setRectCoords(renderer, pieceY * UNIT, pieceX * UNIT, &upLeft);

				}

			}

			if (pieceY + 1 <= maxY) {

				if (board[pieceX - 1][pieceY + 1] == 0) {

					setRectCoords(renderer, (pieceY + 2) * UNIT, pieceX * UNIT, &upRight);

				}

			}

		}

		if (pieceX - 2 >= 0) {

			if (pieceY - 2 >= 0) {

				jumpedPieceUL = pieceTeam == 0 ? p1->getPieceByCoords(pieceX - 1, pieceY - 1) : p0->getPieceByCoords(pieceX - 1, pieceY - 1);
				if (jumpedPieceUL && board[pieceX - 1][pieceY - 1] == 1 && board[pieceX - 2][pieceY - 2] == 0) {

					setRectCoords(renderer, (pieceY - 1) * UNIT, (pieceX - 1) * UNIT, &upLeft);
					jumpUL = 2;

				}

			}

			if (pieceY + 2 <= maxY) {

				jumpedPieceUR = pieceTeam == 0 ? p1->getPieceByCoords(pieceX - 1, pieceY + 1) : p0->getPieceByCoords(pieceX - 1, pieceY + 1);
				if (jumpedPieceUR && board[pieceX - 1][pieceY + 1] == 1 && board[pieceX - 2][pieceY + 2] == 0) {

					setRectCoords(renderer, (pieceY + 3) * UNIT, (pieceX - 1) * UNIT, &upRight);
					jumpUR = 2;

				}

			}

		}

	}

	if (piece->getTeam() == 1 || piece->getIsKing()) {

		if (pieceX + 1 <= maxX) {

			if (pieceY - 1 >= 0) {

				if (board[pieceX + 1][pieceY - 1] == 0) {

					setRectCoords(renderer, pieceY * UNIT, (pieceX + 2) * UNIT, &downLeft);

				}

			}

			if (pieceY + 1 <= maxY) {

				if (board[pieceX + 1][pieceY + 1] == 0) {

					setRectCoords(renderer, (pieceY + 2) * UNIT, (pieceX + 2) * UNIT, &downRight);

				}

			}

		}

		if (pieceX + 2 <= maxX) {

			if (pieceY - 2 >= 0) {

				jumpedPieceDL = pieceTeam == 0 ? p1->getPieceByCoords(pieceX + 1, pieceY - 1) : p0->getPieceByCoords(pieceX + 1, pieceY - 1);
				if (jumpedPieceDL && board[pieceX + 1][pieceY - 1] == 1 && board[pieceX + 2][pieceY - 2] == 0) {

					setRectCoords(renderer, (pieceY - 1) * UNIT, (pieceX + 3) * UNIT, &downLeft);
					jumpDL = 2;

				}

			}

			if (pieceY + 2 <= maxY) {

				jumpedPieceDR = pieceTeam == 0 ? p1->getPieceByCoords(pieceX + 1, pieceY + 1) : p0->getPieceByCoords(pieceX + 1, pieceY + 1);
				if (jumpedPieceDR && board[pieceX + 1][pieceY + 1] == 1 && board[pieceX + 2][pieceY + 2] == 0) {

					setRectCoords(renderer, (pieceY + 3) * UNIT, (pieceX + 3) * UNIT, &downRight);
					jumpDR = 2;

				}

			}

		}

	}
	
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	while (!close) {

		SDL_PollEvent(&event);
		if (event.type == SDL_MOUSEBUTTONDOWN) {

			if (checkRectSelected(&upLeft, event.button.x, event.button.y) == 0) {

				movePieceOnBoard(piece, board, -1 * jumpUL, -1 * jumpUL);
				if (jumpUL == 2) {

					removePiece(renderer, board, jumpedPieceUL, rectList);
					if (pieceTeam == 0) {

						p0->setScore(p0->getScore() + 1);

					} else {

						p1->setScore(p1->getScore() + 1);

					}

				}
				clearGreenRects(renderer, &upLeft, &upRight, &downLeft, &downRight);
				redrawPiece(renderer, piece, &rectList[index], pieceX, pieceY);
				updateScore(renderer, p0, p1);
				return -1;

			} else if (checkRectSelected(&upRight, event.button.x, event.button.y) == 0) {

				movePieceOnBoard(piece, board, -1 * jumpUR, jumpUR);
				if (jumpUR == 2) {

					removePiece(renderer, board, jumpedPieceUR, rectList);
					if (pieceTeam == 0) {

						p0->setScore(p0->getScore() + 1);

					} else {

						p1->setScore(p1->getScore() + 1);

					}

				}
				clearGreenRects(renderer, &upLeft, &upRight, &downLeft, &downRight);
				redrawPiece(renderer, piece, &rectList[index], pieceX, pieceY);
				updateScore(renderer, p0, p1);
				return -1;

			} else if (checkRectSelected(&downLeft, event.button.x, event.button.y) == 0) {

				movePieceOnBoard(piece, board, jumpDL, -1 * jumpDL);
				if (jumpDL == 2) {

					removePiece(renderer, board, jumpedPieceDL, rectList);
					if (pieceTeam == 0) {

						p0->setScore(p0->getScore() + 1);

					}
					else {

						p1->setScore(p1->getScore() + 1);

					}

				}
				clearGreenRects(renderer, &upLeft, &upRight, &downLeft, &downRight);
				redrawPiece(renderer, piece, &rectList[index], pieceX, pieceY);
				updateScore(renderer, p0, p1);
				return -1;

			} else if (checkRectSelected(&downRight, event.button.x, event.button.y) == 0) {

				movePieceOnBoard(piece, board, jumpDR, jumpDR);
				if (jumpDR == 2) {

					removePiece(renderer, board, jumpedPieceDR, rectList);
					if (pieceTeam == 0) {

						p0->setScore(p0->getScore() + 1);

					}
					else {

						p1->setScore(p1->getScore() + 1);

					}

				}
				clearGreenRects(renderer, &upLeft, &upRight, &downLeft, &downRight);
				redrawPiece(renderer, piece, &rectList[index], pieceX, pieceY);
				updateScore(renderer, p0, p1);
				return -1;

			} else {

				clearGreenRects(renderer, &upLeft, &upRight, &downLeft, &downRight);
				return getSelectedPiece(rectList, event.button.x, event.button.y);

			}

		} else if (event.type == SDL_QUIT) {

			return -2;

		}

		SDL_Delay(1000 / FPS);

	}

	clearGreenRects(renderer, &upLeft, &upRight, &downLeft, &downRight);
	SDL_RenderPresent(renderer);

	return -1;

}

/*

This function draws the green rectangles where the selected piece could be moved to

@param SDL_Renderer* renderer: renderer object to draw the green rects
@param int x: x coordinate of rectangle to be drawn
@param int y: y coordinate of rectangle to be drawn
@param SDL_Rect* rect: rectangle object to be drawn

*/
void setRectCoords(SDL_Renderer* renderer, int x, int y, SDL_Rect* rect) {

	rect->x = x;
	rect->y = y;
	SDL_RenderFillRect(renderer, rect);
	SDL_RenderDrawRect(renderer, rect);

}

/*

This function clears the green rectangles from the screen

@param SDL_Renderer* renderer: renderer object to clear the green rectangles with
@param SDL_Rect* rect1: a green rectangle object to be cleared
@param SDL_Rect* rect2: a green rectangle object to be cleared
@param SDL_Rect* rect3: a green rectangle object to be cleared
@param SDL_Rect* rect4: a green rectangle object to be cleared

*/
void clearGreenRects(SDL_Renderer* renderer, SDL_Rect* rect1, SDL_Rect* rect2, SDL_Rect* rect3, SDL_Rect* rect4) {

	if (rect1 && rect1->x != -10) {

		SDL_RenderFillRect(renderer, rect1);
		SDL_RenderDrawRect(renderer, rect1);

	}

	if (rect2 && rect2->x != -10) {

		SDL_RenderFillRect(renderer, rect2);
		SDL_RenderDrawRect(renderer, rect2);

	}

	if (rect3 && rect3->x != -10) {

		SDL_RenderFillRect(renderer, rect3);
		SDL_RenderDrawRect(renderer, rect3);

	}

	if (rect4 && rect4->x != -10) {

		SDL_RenderFillRect(renderer, rect4);
		SDL_RenderDrawRect(renderer, rect4);

	}

	SDL_RenderPresent(renderer);

}