#ifndef DIAMOND_H
#define DIAMOND_H

#include "ObjectOnBoard.h"

class Diamond : public ObjectOnBoard
{
	static int diamondsNumber; // pole przechowujace aktualna liczbe diamentow na planszy
public:
	static int getDiamondsNumber() { return diamondsNumber; }
	static void setDiamondsNumber(int number) { diamondsNumber = number; }

	Diamond(int x, int y) : ObjectOnBoard(x, y)
	{
		++diamondsNumber;
	}

	bool moveDown(Level* level, sf::Sprite[][40], sf::Texture* texture, Player& player); // metoda odpowiedzialna za poruszanie
};

#endif