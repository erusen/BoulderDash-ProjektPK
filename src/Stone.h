#ifndef STONE_H
#define STONE_H

#include "ObjectOnBoard.h"

class Stone : public ObjectOnBoard
{
public:
	Stone(int x, int y) : ObjectOnBoard(x, y) { }
	bool moveDown(Level* level, sf::Sprite[][40], sf::Texture* texture, Player& player);
};

#endif