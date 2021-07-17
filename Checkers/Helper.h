/*

This function creates a yes/no option window for the user to respond to

@param const char* title: the title for the popup window to have
@param const char* question: the question to be confirmed/denied by the user

@return int: returns 0 when the "no" option is selected or an error occurrs, returns 1 when the "yes" option is selected

*/
int createModal(const char* title, const char* question, const char* firstResp, const char* secondResp) {

	int* titleWidth = (int*)malloc(sizeof(int));
	int* yesWidth = (int*)malloc(sizeof(int));
	int* noWidth = (int*)malloc(sizeof(int));
	int close = 0, toRet = 0, popWidth = 0;

	if (!(titleWidth && yesWidth && noWidth)) {

		return 0;

	}

	SDL_Rect* yesRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	SDL_Rect* noRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	SDL_Color blackText = { 0, 0, 0 };
	SDL_Event event;

	TTF_Font* largeText = TTF_OpenFont("Raleway-Thin.ttf", 30);
	TTF_Font* smallText = TTF_OpenFont("Raleway-Thin.ttf", 20);

	TTF_SizeText(largeText, question, titleWidth, NULL);
	TTF_SizeText(smallText, firstResp, yesWidth, NULL);
	if (secondResp) {

		TTF_SizeText(smallText, secondResp, noWidth, NULL);

	}

	popWidth = ((((*titleWidth) / 100) + 1) * 100);

	SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, popWidth, 300, SDL_WINDOW_ALWAYS_ON_TOP);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderClear(renderer);

	addText(renderer, largeText, question, blackText, (popWidth / 2) - ((*titleWidth) / 2), 50, yesRect);
	if (secondResp) {

		addText(renderer, smallText, firstResp, blackText, 100 - ((*yesWidth) / 2), 200, yesRect);
		addText(renderer, smallText, secondResp, blackText, (popWidth - 100) - ((*noWidth) / 2), 200, noRect);

	} else {

		addText(renderer, smallText, firstResp, blackText, (popWidth / 2) - ((*yesWidth) / 2), 200, yesRect);

	}

	SDL_RenderPresent(renderer);

	if (!yesRect || !noRect) {

		free(titleWidth);
		free(yesWidth);
		free(noWidth);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return 0;

	}

	while (close != 1) {

		SDL_PollEvent(&event);
		if (event.type == SDL_MOUSEBUTTONDOWN) {

			if (checkRectSelected(yesRect, event.button.x, event.button.y) == 0) {

				close = 1;
				toRet = 1;

			} else if (checkRectSelected(noRect, event.button.x, event.button.y) == 0) {

				close = 1;
				toRet = 0;

			}

		}

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {

			close = 1;
			toRet = 0;

		}

		SDL_Delay(1000 / FPS);

	}

	free(yesRect);
	free(noRect);
	free(titleWidth);
	free(yesWidth);
	free(noWidth);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return toRet;

}

/*

This function adds text to the renderer passed into it

@param SDL_Renderer* renderer: A pointer to the renderer where text is to be added
@param TTF_Font* font: A pointer to the font style and size that is to be used for the text
@param const char* text: A pointer to the text to be written to the renderer
@param SDL_Color color: The color that the text is to be printed in
@param int x: The x coordinate of the rectangle for the text to be put in
@param int y: The y coordinate of the rectangle for the text to be put in
@param SDL_Rect* rect: A pointer to the rectangle object holding the text

*/
void addText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y, SDL_Rect* rect) {

	SDL_Surface* tempSurf = TTF_RenderText_Solid(font, text, color);
	if (!tempSurf || !rect) {

		std::cout << "Failed to render text: \"" << text << "\"" << std::endl;
		return;

	}

	SDL_Texture* tempTex = SDL_CreateTextureFromSurface(renderer, tempSurf);
	rect->x = x;
	rect->y = y;
	rect->w = tempSurf->w;
	rect->h = tempSurf->h;
	SDL_RenderCopy(renderer, tempTex, NULL, rect);

	SDL_DestroyTexture(tempTex);

}

/*

This function is a helper funtion to change the coordinates of a piece and write them to the board int 2D array

@param GamePiece* piece: piece that is being moved
@param int** board: 2D int array tracking where all pieces are
@param int changeInX: int signifying where the piece is to be moved on the x plane
@param int changeInY: int signifying where the piece is to be moved on the y plane

*/
void movePieceOnBoard(GamePiece* piece, int** board, int changeInX, int changeInY) {

	int x = piece->getX(), y = piece->getY();

	piece->setX(x + changeInX);
	piece->setY(y + changeInY);

	if ((piece->getX() == 0 && piece->getTeam() == 0) || (piece->getX() == maxX && piece->getTeam() == 1)) {

		piece->setIsKing(true);

	}

	board[x][y] = 0;
	board[x + changeInX][y + changeInY] = 1;

}

/*

This function frees the memory allocated to the 2D array tracking board positions

@param int** gameBoard: the 2D array holding all board positions

*/
void freeBoard(int** gameBoard) {

	if (!gameBoard) {

		return;

	}

	for (int i = 0; i < 8; i++) {

		free(gameBoard[i]);

	}

	free(gameBoard);

}

/*

This function quits SDL, TTF, and IMG given the chosen flag, also deletes/frees game objects

@param int quitFlag: flag signalling what services to quit (-1 : quit nothing
															 0 : quit nothing and free board
															 1 : quit SDL and free board
															 2 : quit SDL and IMG and free board
															 3 : quit SDL, IMG, and TTF and free board)
@param int** gameBoard: board showing pieces to be freed
@param Player* p0: player object to be deleted
@param Player* p1: player object to be deleted

*/
void tearDown(int quitFlag, int** gameBoard, Player* p0, Player* p1) {

	switch (quitFlag) {

	case -1:
		break;
	case 3: 
		TTF_Quit();
	case 2:
		IMG_Quit();
	case 1:
		SDL_Quit();
	default:
		freeBoard(gameBoard);
		break;
	}

	if (p0) {

		p0->destroyPieces();
		delete(p0);

	}

	if (p1) {

		p1->destroyPieces();
		delete(p1);

	}

}

void updateScore(SDL_Renderer* renderer, Player* p0, Player* p1) {

	SDL_SetRenderDrawColor(renderer, 200, 211, 201, 255);

	TTF_Font* font = TTF_OpenFont("Raleway-Thin.ttf", 30);
	int redWidth, blueWidth, redHeight;
	SDL_Rect blueScore, redScore;
	SDL_Rect tempRect;
	SDL_Color red = { 255, 0, 0 }, blue = { 0, 0, 255 };

	std::string str = "Red Score = " + std::to_string(p0->getScore());
	TTF_SizeText(font, str.c_str(), &redWidth, &redHeight);
	redScore.x = (GAMESCREENWIDTH / 2) - (redWidth / 2) - 50;
	redScore.y = (GAMESCREENHEIGHT - redHeight - 50);
	redScore.w = redWidth + 100;
	redScore.h = redHeight + 10;

	SDL_RenderFillRect(renderer, &redScore);
	SDL_RenderDrawRect(renderer, &redScore);

	addText(renderer, font, str.c_str(), red, (GAMESCREENWIDTH / 2) - (redWidth / 2), (GAMESCREENHEIGHT - redHeight - 50), &tempRect);

	str = "Blue Score = " + std::to_string(p1->getScore());
	TTF_SizeText(font, str.c_str(), &blueWidth, NULL);
	blueScore.x = (GAMESCREENWIDTH / 2) - (blueWidth / 2) - 50;
	blueScore.y = 40;
	blueScore.w = blueWidth + 100;
	blueScore.h = redHeight + 10;

	SDL_RenderFillRect(renderer, &blueScore);
	SDL_RenderDrawRect(renderer, &blueScore);

	addText(renderer, font, str.c_str(), blue, (GAMESCREENWIDTH / 2) - (blueWidth / 2), 50, &tempRect);

	SDL_RenderPresent(renderer);

}