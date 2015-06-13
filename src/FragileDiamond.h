#ifndef FRAGILE_DIAMOND_H
#define FRAGILE_DIAMOND_H

#include "Diamond.h"
#include "SpecialObject.h"
#include "Level.h"
#include "Player.h"
#include <SFML\Graphics.hpp>
#include <list>

class FragileDiamond : public Diamond
{
private:
	bool prevIsMove;
	bool currentIsMove;
	bool moveDownDiamond(Level* level, sf::Sprite[][40], sf::Texture* texture, Player& player); // pomocnicza metoda wywolywana wewnatrz moveDown

public:
	FragileDiamond(int x, int y) : Diamond(x, y)
	{
		prevIsMove = false;
		currentIsMove = false;
	}
	bool moveDown(Level* level, sf::Sprite[][40], sf::Texture* texture, Player& player); // spadanie diamentu
	bool breakDiamond(std::list<ObjectOnBoard*> objectOnBoard, Player& player, sf::Sprite sprite[][40]); // metoda sprawdzajaca czy diament zostal zniszczony
};

#endif