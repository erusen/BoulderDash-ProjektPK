#include "FragileDiamond.h"

bool FragileDiamond::moveDownDiamond(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	int currentX = getX();
	int currentY = getY();

	sf::FloatRect objectBox = sprite[currentY][currentX].getGlobalBounds();
	sf::FloatRect playerBox = player.getBoundingBox();

	objectBox.top += (SPEED * 180);

	int nextX = (objectBox.left) / level->TILE_WIDTH;	// obliczenie nowych wspolrzednych obiektu x,y
	int nextY = (objectBox.top + level->TILE_HEIGHT) / level->TILE_HEIGHT;


	if (level->board[nextY][nextX].type == Level::NONE)
	{
		isMoving = true;
		sprite[currentY][currentX].move(0, SPEED * 180);	// przesuniecie diamentu

		// pobranie zaktualizowanej pozycji obiektu i obliczenie nowej wspolrzednej
		objectBox = sprite[currentY][currentX].getGlobalBounds();
		objectBox.top += (level->TILE_HEIGHT / 2);
		nextY = objectBox.top / level->TILE_HEIGHT;

		if (currentY < nextY)	// sprawdzenie czy przesuniecie tekstury diamentu spowodowalo jego przemieszczenie do nastepnej kratki
		{
			// ustawienie starego pola jako puste
			level->board[nextY - 1][nextX].type = Level::NONE;

			// ustawienie nowego pola jako zajete przez diament
			level->board[nextY][nextX].type = Level::FRAGILE_DIAMOND;

			// aktualizacja pozycji tekstury
			sprite[nextY][nextX].setTexture(texture[Level::FRAGILE_DIAMOND]);
			sprite[nextY][nextX].setPosition(nextX * Level::TILE_WIDTH, nextY * Level::TILE_HEIGHT);

			// aktualizacja pozycji obiektu
			setX(nextX);
			setY(nextY);
		}

		return true;
	}
	else if (isMoving == true)
	{
		isMoving = false;
		return false;
	}

	return false;
}

bool FragileDiamond::moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	prevIsMove = currentIsMove;
	moveDownDiamond(level, sprite, texture, player);
	currentIsMove = this->isMoving;

	if (currentIsMove == true)
	{
		return true;
	}

	return false;
}

bool FragileDiamond::breakDiamond(std::list<ObjectOnBoard*> objectOnBoard, Player& player, sf::Sprite sprite[][40])
{

	if (currentIsMove == false && prevIsMove == true)	// testuje czy diament spadl na ziemie
	{
		return false;
	}

	int appear = 0;
	for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end(); iter++)	// zlicza obiekty o jednakowych x,y 
	{
		if ((*iter)->getX() == this->getX() && (*iter)->getY() == this->getY())
		{
			++appear;
		}
	}

	if (appear >= 2)	// jesli kamien spadl na diament to sa przynajmniej 2 obiekty o jednakowym x,y
	{
		return false;
	}

	return true;
}