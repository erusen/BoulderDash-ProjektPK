#ifndef OBJECT_ON_BOARD
#define OBJECT_ON_BOARD

#include <array>
#include "Level.h"
#include "Player.h"
#include <SFML\Graphics.hpp>

using std::array;

class ObjectOnBoard
{
protected:
	array<int, 2> coordinates;	// wspolrzedne x,y obiektu
	bool isMoving = false;
	const double SPEED = 0.008;

public:
	ObjectOnBoard(int x, int y) 
	{
		coordinates[0] = x;
		coordinates[1] = y;
	} 
	int getX() { return coordinates[0]; }
	int getY() { return coordinates[1]; }
	void setX(int x) { coordinates[0] = x; }
	void setY(int y) { coordinates[1] = y; }

	virtual bool moveDown(Level* level, sf::Sprite [][40], sf::Texture* texture, Player& player) = 0; 
};

#endif