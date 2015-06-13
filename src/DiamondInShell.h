#ifndef DIAMOND_IN_SHELL_H
#define DIAMOND_IN_SHELL_H

#include "Diamond.h"
#include "SpecialObject.h"
#include "Level.h"
#include "Player.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <string>

using std::string;

class DiamondInShell : public Diamond
{
private:
	bool prevIsStoneAbove;
	bool currentIsStoneAbove;
	bool shell; // informacja czy diament jest w muszli czy nie
public:
	DiamondInShell(int x, int y) : Diamond(x, y)
	{
		prevIsStoneAbove = false;
		currentIsStoneAbove = false;
		shell = true;
	}
	bool getShell() { return shell; }
	bool breakShell(std::list<ObjectOnBoard*> objectOnBoard, Level* level, sf::Sprite[][40], sf::Texture* texture); // metoda sprawdzajaca czy muszla ma zostac zniszczona
	bool moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player); // poruszanie sie obiektu
};

#endif