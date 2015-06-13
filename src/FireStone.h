#ifndef FIRE_STONE_H
#define FIRE_STONE_H

#include "Stone.h"

class FireStone : public Stone
{
public:
	bool getIsExplosion() { return isExplosion; }
	FireStone(int x, int y) : Stone(x, y)
	{
		prevIsMove = false;
		currentIsMove = false;
		isExplosion = false;
	}
	bool moveDown(Level* level, sf::Sprite[][40], sf::Texture* texture, Player& player);

private:
	bool isExplosion; // informacja czy dany obiekt eksplodowal
	bool prevIsMove;
	bool currentIsMove;
	void explode(Level* level, Player& player); // funkcja odpowiedzialna za niszczenie otoczenia
	bool moveDownStone(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player); // spadanie kamienia
};

#endif