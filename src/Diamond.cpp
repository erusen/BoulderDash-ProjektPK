#include "Diamond.h"

int Diamond::diamondsNumber = 0;

bool Diamond::moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
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
			level->board[nextY][nextX].type = Level::DIAMOND;

			// aktualizacja pozycji tekstury
			sprite[nextY][nextX].setTexture(texture[Level::DIAMOND]);
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