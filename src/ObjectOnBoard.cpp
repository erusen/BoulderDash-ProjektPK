#include "ObjectOnBoard.h"

/*bool ObjectOnBoard::moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	int currentX = getX();
	int currentY = getY();

	string diamondType = "class Diamond";
	string fragileDiamondType = "class FragileDiamond";
	string diamondInShellType = "class DiamondInShell";
	string stoneType = "class Stone";
	string fireStoneType = "class FireStone";
	string cloningStoneType = "class CloningStone";

	sf::FloatRect ObjectBox = sprite[currentY][currentX].getGlobalBounds();
	sf::FloatRect PlayerBox = player.getBoundingBox();

	ObjectBox.left += (0); //dzielimy na 10 aby bardziej  zbli¿yæ siê do œciany
	ObjectBox.top += ((speed * 180));

	int x = (ObjectBox.left) / level->TILE_WIDTH;
	int y = (ObjectBox.top + level->TILE_HEIGHT) / level->TILE_HEIGHT;
	
	if (!ObjectBox.intersects(PlayerBox) && (currentY < Level::HEIGHT) && ((level->board[y][x].type == Level::NONE) || (level->board[y][x].type == Level::FALLING) || (level->board[y][x].type == Level::FRAGILE_DIAMOND)))
	{
		isMoving = true;
		sprite[currentY][currentX].move(0, speed * 180);
		level->board[currentY][currentX].type = Level::FALLING;
		level->board[currentY][currentX].isWall = false;
		return true;
	}
	else if (isMoving == true)
	{
		isMoving = false;
		
		//if (level->board[currentY][currentX].type != Level::STONE)
		level->board[currentY][currentX].type = Level::NONE;
		
		if (typeid(*this).name() == stoneType)
		{
			level->board[y - 1][x].type = Level::STONE;
			level->board[y - 1][x].isWall = true;		// true
			sprite[y - 1][x].setTexture(texture[3]);
		}
		else if (typeid(*this).name() == diamondType)
		{
			level->board[y - 1][x].type = Level::DIAMOND;
			level->board[y - 1][x].isWall = false;		// false
			sprite[y - 1][x].setTexture(texture[4]);
		}
		else if (typeid(*this).name() == fragileDiamondType)
		{
			level->board[y - 1][x].type = Level::FRAGILE_DIAMOND;
			level->board[y - 1][x].isWall = false;		// false
			sprite[y - 1][x].setTexture(texture[6]);
		}
		else if (typeid(*this).name() == diamondInShellType)
		{
			level->board[y - 1][x].type = Level::DIAMOND_IN_SHELL;
			if (level->board[currentY][currentX].isWall == true)
			{
				level->board[y - 1][x].isWall = true;		// false
				sprite[y - 1][x].setTexture(texture[7]);
			}
			else
			{
				level->board[y - 1][x].isWall = false;		// false
				sprite[y - 1][x].setTexture(texture[8]);
			}	
		}
		else if (typeid(*this).name() == fireStoneType)
		{
			level->board[y - 1][x].type = Level::FIRE_STONE;
			level->board[y - 1][x].isWall = true;		// true
			sprite[y - 1][x].setTexture(texture[9]);
		}
		else if (typeid(*this).name() == cloningStoneType)
		{
			level->board[y - 1][x].type = Level::CLONING_STONE;
			level->board[y - 1][x].isWall = true;		// true
			sprite[y - 1][x].setTexture(texture[5]);
		}

		

		//sprite[currentY][currentX].setTexture(texture[0]);
		
		sprite[y-1][x].setPosition(x*Level::TILE_WIDTH, (y-1)*Level::TILE_HEIGHT);
		setX(x);
		setY(y-1);

		return false;
	}
	

	return false;
}*/