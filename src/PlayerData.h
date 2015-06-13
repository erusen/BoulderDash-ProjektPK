#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <string>

using namespace std;

class PlayerData
{
	string name;
	int score;
public:
	string getName() { return name; }
	void setName(string name) { this->name = name; }
	int getScore() { return score; }
	void setScore(int score) { this->score = score; }

	PlayerData(string name, int score)
	{
		setName(name);
		setScore(score);
	}
};

#endif
