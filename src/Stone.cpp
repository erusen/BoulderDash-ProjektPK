#include "Stone.h"

bool Stone::moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	int currentX = getX();
	int currentY = getY();

	sf::FloatRect objectBox = sprite[currentY][currentX].getGlobalBounds();
	
	sf::FloatRect playerBox = player.getBoundingBox();
	int playerX = playerBox.left / Level::TILE_WIDTH;
	int playerY = playerBox.top / Level::TILE_HEIGHT;

	objectBox.top += (SPEED * 180); // obliczenie nowej pozycji tekstury obiektu jaka mialaby po przesunieciu

	int nextX = (objectBox.left) / level->TILE_WIDTH;	// obliczenie nowych wspolrzednych obiektu x,y
	int nextY = (objectBox.top + level->TILE_HEIGHT) / level->TILE_HEIGHT;

	if (objectBox.intersects(playerBox) && isMoving) // sprawdzenie czy kamien przecina sie z graczem i jesli tak to ustawienie statusu gracza jako martwy
	{
		player.setIsDead(true);
	}


	if (!objectBox.intersects(playerBox) && ((level->board[nextY][nextX].type == Level::NONE) || (level->board[nextY][nextX].type == Level::FRAGILE_DIAMOND)))
	{
		isMoving = true;
		sprite[currentY][currentX].move(0, SPEED * 180);	// przesuniecie kamienia

		// pobranie zaktualizowanej pozycji obiektu i obliczenie nowej wspolrzednej
		objectBox = sprite[currentY][currentX].getGlobalBounds();
		objectBox.top += (level->TILE_HEIGHT / 2);
		nextY = objectBox.top / level->TILE_HEIGHT;

		if (currentY < nextY)	// sprawdzenie czy przesuniecie tekstury kamienia spowodowalo jego przemieszczenie do nastepnej kratki
		{
			// ustawienie starego pola jako puste
			level->board[nextY - 1][nextX].isWall = false;
			level->board[nextY - 1][nextX].type = Level::NONE;

			// ustawienie nowego pola jako zajete przez kamien
			level->board[nextY][nextX].type = Level::STONE;
			level->board[nextY][nextX].isWall = true;

			// aktualizacja pozycji tekstury
			sprite[nextY][nextX].setTexture(texture[Level::STONE]);
			sprite[nextY][nextX].setPosition(nextX * Level::TILE_WIDTH, nextY * Level::TILE_HEIGHT);
			
			// aktualizacja pozycji obiektu
			setX(nextX);
			setY(nextY);
		}

		return true;
	}

	// ruch kamienia na skos w prawo
	else if (level->board[currentY][nextX + 1].type == Level::NONE &&  level->board[currentY + 1][nextX + 1].type == Level::NONE && !((currentY + 1) == playerY && (nextX + 1) == playerX) && !(currentY == playerY && (nextX + 1) == playerX))
	{
		// ustawienie starego pola jako puste
		level->board[currentY][currentX].isWall = false;
		level->board[currentY][currentX].type = Level::NONE;

		// ustawienie nowego pola jako zajete przez kamien
		level->board[nextY - 1][nextX + 1].type = Level::STONE;
		level->board[nextY - 1][nextX + 1].isWall = true;
		
		// aktualizacja pozycji tekstury
		sprite[nextY - 1][nextX + 1].setTexture(texture[Level::STONE]);
		sprite[nextY - 1][nextX + 1].setPosition((nextX + 1) * Level::TILE_WIDTH, (nextY - 1) * Level::TILE_HEIGHT);

		// aktualizacja pozycji obiektu
		setX(nextX + 1);
		setY(nextY - 1);

		return true;
	}

	// ruch kamienia na skos w lewo
	else if (level->board[currentY][nextX - 1].type == Level::NONE &&  level->board[currentY + 1][nextX - 1].type == Level::NONE && !((currentY + 1) == playerY && (nextX - 1) == playerX) && !(currentY == playerY && (nextX - 1) == playerX))
	{
		// ustawienie starego pola jako puste
		level->board[currentY][currentX].isWall = false;
		level->board[currentY][currentX].type = Level::NONE;

		// ustawienie nowego pola jako zajete przez kamien
		level->board[nextY - 1][nextX - 1].type = Level::STONE;
		level->board[nextY - 1][nextX - 1].isWall = true;

		// aktualizacja pozycji tekstury
		sprite[nextY - 1][nextX - 1].setTexture(texture[Level::STONE]);
		sprite[nextY - 1][nextX - 1].setPosition((nextX - 1) * Level::TILE_WIDTH, (nextY - 1) * Level::TILE_HEIGHT);

		// aktualizacja pozycji obiektu
		setX(nextX - 1);
		setY(nextY - 1);

		return true;
	}

	// zatrzymanie spadajacego kamienia
	else if (isMoving == true)
	{
		isMoving = false;
		return false;
	}

	return false;
}