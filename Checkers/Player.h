/*

Header file containing the class definition for the Player class

Name: Noah Hinderle
Email: noah.hinderle@gmail.com
Date: July 7, 2021

*/

class Player {

	private:
		GamePiece* pieces[12];
		int score;
		int playerNum;
		bool isTurn;

	public:
		Player(int playerNum) {

			setScore(0);
			setPlayerNum(playerNum);
			setPieces(playerNum);
			setIsTurn(false);

		}

		void setScore(int score) {

			this->score = score;

		}

		int getScore() {

			return score;

		}

		int setPlayerNum(int playerNum) {

			if (playerNum == 0 || playerNum == 1) {

				this->playerNum = playerNum;
				return 0;

			}

			return -1;

		}

		int getPlayerNum() {

			return playerNum;

		}

		int setPieces(int playerNum) {

			GamePiece* tempPiece;
			int topLine = 0;
			int index = 0;

			if (playerNum == 0) {

				topLine = 5;

				for (int i = 0; i < 8; i++) {

					if (i % 2 == 0) {

						tempPiece = new GamePiece(topLine, i, playerNum);
						pieces[index++] = tempPiece;
						tempPiece = new GamePiece(topLine + 2, i, playerNum);
						pieces[index++] = tempPiece;

					} else {

						tempPiece = new GamePiece(topLine + 1, i, playerNum);
						pieces[index++] = tempPiece;

					}

				}

				return 0;

			} else if (playerNum == 1) {

				topLine = 0;

				for (int i = 0; i < 8; i++) {

					if (i % 2 == 1) {

						tempPiece = new GamePiece(topLine, i, playerNum);
						pieces[index++] = tempPiece;
						tempPiece = new GamePiece(topLine + 2, i, playerNum);
						pieces[index++] = tempPiece;

					} else {

						tempPiece = new GamePiece(topLine + 1, i, playerNum);
						pieces[index++] = tempPiece;

					}

				}

				return 0;

			}

			return -1;

		}

		GamePiece** getPieces() {

			return pieces;

		}

		void destroyPieces() {

			for (int i = 0; i < 12; i++) {

				delete(pieces[i]);

			}

		}

		void setIsTurn(bool isTurn) {

			this->isTurn = isTurn;

		}

		bool getIsTurn() {

			return isTurn;

		}

};
