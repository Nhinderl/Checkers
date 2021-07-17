/*

Header file containing the class definition for the GamePiece class

Name: Noah Hinderle
Email: noah.hinderle@gmail.com
Date: July 16, 2021

*/

class GamePiece {

	friend class boost::serialization::access;

private:
	int x;
	int y;
	bool inPlay;
	int team;
	bool isKing;
	int index = -1;

public:
	GamePiece() {

		setX(0);
		setY(0);
		setInPlay(false);
		setTeam(0);
		setIsKing(false);

	}

	GamePiece(int x, int y, int team) {

		setX(x);
		setY(y);
		setInPlay(true);
		setIsKing(false);
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

	void setIsKing(bool isKing) {

		this->isKing = isKing;

	}

	bool getIsKing() {

		return isKing;

	}

	void setIndex(int index) {

		if (index >= 0 && index <= 23) {

			this->index = index;

		}

	}

	int getIndex() {

		return index;

	}

	template<class Archive>
	void serialize(Archive& arc, const unsigned int version) {

		arc& BOOST_SERIALIZATION_NVP(x);
		arc& BOOST_SERIALIZATION_NVP(y);
		arc& BOOST_SERIALIZATION_NVP(inPlay);
		arc& BOOST_SERIALIZATION_NVP(team);
		arc& BOOST_SERIALIZATION_NVP(isKing);
		arc& BOOST_SERIALIZATION_NVP(index);

	}

};
