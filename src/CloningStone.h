#ifndef CLONING_STONE_H
#define CLONING_STONE_H

#include "Stone.h"

class CloningStone : public Stone
{
public:
	bool getIsCloning() { return isCloning; }

	CloningStone(int x, int y) : Stone(x, y)
	{
		isCloning = false;
		prevIsMove = false;
		currentIsMove = false;
	}

	bool moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player); // metoda odpowiedzialna za poruszanie

private:
	bool isCloning; // pole informujace czy dany obiekt uzyl swojej zdolnosci do tworzenia kamieni
	bool prevIsMove; // przechowuje informacje o poprzednim stanie obiektu czy byl w ruchu
	bool currentIsMove; // przechowuje aktualne informacje o tym czy obiekt jest w ruchu
	void clone(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player); // metoda ustawia sasiadujace pola typu GRASS i NONE na typ STONE
	bool moveDownStone(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player); // pomocnicza metoda wywolywana wewnatz moveDown
};

#endif