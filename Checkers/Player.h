/*

Header file containing the class definition for the Player class

Name: Noah Hinderle
Email: noah.hinderle@gmail.com
Date: July 16, 2021

*/

class Player {

	friend class boost::serialization::access;

	private:
		GamePiece* pieces[12];
		int score;
		int playerNum;
		bool isTurn;

	public:
		Player() {

			setScore(0);
			setPlayerNum(1);
			setPieces(1);
			setIsTurn(false);

		}

		Player(int playerNum) {

			setScore(0);
			setPlayerNum(playerNum);
			setPieces(playerNum);
			setIsTurn(playerNum);

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
						tempPiece->setIndex(index);
						pieces[index++] = tempPiece;
						tempPiece = new GamePiece(topLine + 2, i, playerNum);
						tempPiece->setIndex(index);
						pieces[index++] = tempPiece;

					} else {

						tempPiece = new GamePiece(topLine + 1, i, playerNum);
						tempPiece->setIndex(index);
						pieces[index++] = tempPiece;

					}

				}

				return 0;

			} else if (playerNum == 1) {

				topLine = 0;

				for (int i = 0; i < 8; i++) {

					if (i % 2 == 1) {

						tempPiece = new GamePiece(topLine, i, playerNum);
						tempPiece->setIndex(index + 12);
						pieces[index++] = tempPiece;
						tempPiece = new GamePiece(topLine + 2, i, playerNum);
						tempPiece->setIndex(index + 12);
						pieces[index++] = tempPiece;

					} else {

						tempPiece = new GamePiece(topLine + 1, i, playerNum);
						tempPiece->setIndex(index + 12);
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

		void setIsTurn(int playerNum) {

			isTurn = playerNum == 0 ? true : false;

		}

		bool getIsTurn() {

			return isTurn;

		}

		GamePiece* getPieceByCoords(int x, int y) {

			for (int i = 0; i < 12; i++) {

				if (pieces[i] && pieces[i]->getX() == x && pieces[i]->getY() == y) {

					return pieces[i];

				}

			}

			return NULL;

		}

		template<class Archive>
		void serialize(Archive& arc, const unsigned int version) {

			arc& BOOST_SERIALIZATION_NVP(score);
			arc& BOOST_SERIALIZATION_NVP(playerNum);
			arc& BOOST_SERIALIZATION_NVP(isTurn);
			arc& BOOST_SERIALIZATION_NVP(pieces);

		}

};
