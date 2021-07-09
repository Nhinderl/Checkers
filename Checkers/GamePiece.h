/*

Header file containing the class definition for the GamePiece class

Name: Noah Hinderle
Email: noah.hinderle@gmail.com
Date: July 7, 2021

*/

class GamePiece {

private:
	int x;
	int y;
	bool inPlay;
	int team;

public:
	GamePiece(int x, int y, int team) {

		setX(x);
		setY(y);
		setInPlay(true);
		setTeam(team);

	}

	int setX(int x) {

		if (x >= 0 && x <= 7) {

			this->x = x;
			return 0;

		}

		return -1;

	}

	int getX() {

		return x;

	}

	int setY(int y) {

		if (y >= 0 && y <= 7) {

			this->y = y;
			return 0;

		}

		return -1;

	}

	int getY() {

		return y;

	}

	void setInPlay(bool inPlay) {

		this->inPlay = inPlay;

	}

	bool getInPlay() {

		return inPlay;

	}

	int setTeam(int team) {

		if (team == 1 || team == 0) {

			this->team = team;
			return 0;

		}

		return -1;

	}

	int getTeam() {

		return team;

	}

};
