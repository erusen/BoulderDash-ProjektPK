#include "DiamondInShell.h"

bool DiamondInShell::moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	int currentX = getX();
	int currentY = getY();

	sf::FloatRect objectBox = sprite[currentY][currentX].getGlobalBounds();
	sf::FloatRect playerBox = player.getBoundingBox();

	objectBox.top += (SPEED * 180);

	int nextX = (objectBox.left) / level->TILE_WIDTH;	// obliczenie nowych wspolrzednych obiektu x,y
	int nextY = (objectBox.top + level->TILE_HEIGHT) / level->TILE_HEIGHT;

	if (!objectBox.intersects(playerBox) && (level->board[nextY][nextX].type == Level::NONE))
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

			// ustawienie nowego pola jako zajete przez diament i aktualizacja tekstury
			if (shell)
			{
				level->board[nextY - 1][nextX].isWall = false;
				level->board[nextY][nextX].isWall = true;
				level->board[nextY][nextX].type = Level::DIAMOND_IN_SHELL;
				sprite[nextY][nextX].setTexture(texture[Level::DIAMOND_IN_SHELL]);
			}
			else
			{
				level->board[nextY][nextX].type = Level::DIAMOND_WITHOUT_SHELL;
				sprite[nextY][nextX].setTexture(texture[Level::DIAMOND_WITHOUT_SHELL]);
			}

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

bool DiamondInShell::breakShell(std::list<ObjectOnBoard*> objectOnBoard, Level* level, sf::Sprite sprite[][40], sf::Texture* texture)
{
	int appear = 0;
	for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end(); iter++)	// zlicza obiekty o jednakowych x,y 
	{
		string type = "class Stone";
		if ((*iter)->getX() == this->getX() && (*iter)->getY() == this->getY() - 1 && typeid(**iter).name() == type) // szukanie kamienia ktory znajduje sie nad diamentem
		{
			currentIsStoneAbove = true;
			if (prevIsStoneAbove == false && currentIsStoneAbove == true) // jesli wczesniej nie bylo kamienia i teraz sie pojawil to zniszcz muszle
			{
				++appear;
				shell = false;
				level->board[getY()][getX()].isWall = false;
				level->board[getY()][getX()].type = Level::DIAMOND_WITHOUT_SHELL;
				sprite[getY()][getX()].setTexture(texture[Level::DIAMOND_WITHOUT_SHELL]);
			}
		}
	}

	if (appear == 0) // jesli nie pojawil sie nad diamentem kamien
	{
		prevIsStoneAbove = currentIsStoneAbove;
		currentIsStoneAbove = false;
	}
	
	return true;
}