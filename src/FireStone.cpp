#include "FireStone.h"

bool FireStone::moveDownStone(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	int currentX = getX();
	int currentY = getY();

	sf::FloatRect objectBox = sprite[currentY][currentX].getGlobalBounds();
	sf::FloatRect playerBox = player.getBoundingBox();

	objectBox.top += (SPEED * 180);

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
			level->board[nextY][nextX].type = Level::FIRE_STONE;
			level->board[nextY][nextX].isWall = true;

			// aktualizacja pozycji tekstury
			sprite[nextY][nextX].setTexture(texture[Level::FIRE_STONE]);
			sprite[nextY][nextX].setPosition(nextX * Level::TILE_WIDTH, nextY * Level::TILE_HEIGHT);

			// aktualizacja pozycji obiektu
			setX(nextX);
			setY(nextY);
		}

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

bool FireStone::moveDown(Level* level, sf::Sprite sprite[][40], sf::Texture* texture, Player& player)
{
	currentIsMove = moveDownStone(level, sprite, texture, player); // jesli metoda przesunie kamien zwroci true, a jesli nie to false

	if (prevIsMove == true && currentIsMove == false) // sprawdzenie czy kamien poprzednio spadal i teraz sie zatrzymal jesli tak to musi eksplodowac
	{
		explode(level, player);
	}

	prevIsMove = currentIsMove;

	return true;
}

void FireStone::explode(Level* level, Player& player)
{
	int currentX = this->getX();
	int currentY = this->getY();

	sf::FloatRect box(player.getBoundingBox());
	int playerX = (box.left + (level->TILE_WIDTH / 2)) / level->TILE_WIDTH;
	int playerY = (box.top + (level->TILE_HEIGHT / 2)) / level->TILE_HEIGHT;

	// sprawdzenie wszystkich 8 sasiadujacych pol
	// gorne lewe
	if (level->board[currentY - 1][currentX - 1].type == Level::NONE || level->board[currentY - 1][currentX - 1].type == Level::GRASS)
	{
		level->board[currentY - 1][currentX - 1].type = Level::NONE; // jesli pole bylo trawa to ustawiane jest na puste
		
		if (playerX == currentX - 1 && playerY == currentY - 1)	// jesli gracz znajdowal sie w obszarze wybuchu to jego status ustawiany jest na martwy
		{
			player.setIsDead(true);
		}
	}

	// gorne srodkowe
	if (level->board[currentY - 1][currentX].type == Level::NONE || level->board[currentY - 1][currentX].type == Level::GRASS)
	{
		level->board[currentY - 1][currentX].type = Level::NONE;

		if (playerX == currentX && playerY == currentY - 1)
		{
			player.setIsDead(true);
		}
	}

	// gorne prawe
	if (level->board[currentY - 1][currentX + 1].type == Level::NONE || level->board[currentY - 1][currentX + 1].type == Level::GRASS)
	{
		level->board[currentY - 1][currentX + 1].type = Level::NONE;

		if (playerX == currentX + 1 && playerY == currentY - 1)
		{
			player.setIsDead(true);
		}
	}

	// srodkowe lewe
	if (level->board[currentY][currentX - 1].type == Level::NONE || level->board[currentY][currentX - 1].type == Level::GRASS)
	{
		level->board[currentY][currentX - 1].type = Level::NONE;

		if (playerX == currentX - 1 && playerY == currentY)
		{
			player.setIsDead(true);
		}
	}

	// srodkowe prawe
	if (level->board[currentY][currentX + 1].type == Level::NONE || level->board[currentY][currentX + 1].type == Level::GRASS)
	{
		level->board[currentY][currentX + 1].type = Level::NONE;

		if (playerX == currentX + 1 && playerY == currentY)
		{
			player.setIsDead(true);
		}
	}

	// dolne lewe
	if (level->board[currentY + 1][currentX - 1].type == Level::NONE || level->board[currentY + 1][currentX - 1].type == Level::GRASS)
	{
		level->board[currentY + 1][currentX - 1].type = Level::NONE;

		if (playerX == currentX - 1 && playerY == currentY + 1)
		{
			player.setIsDead(true);
		}
	}

	// dolne srodkowe
	if (level->board[currentY + 1][currentX].type == Level::NONE || level->board[currentY + 1][currentX].type == Level::GRASS)
	{
		level->board[currentY + 1][currentX].type = Level::NONE;

		if (playerX == currentX && playerY == currentY + 1)
		{
			player.setIsDead(true);
		}
	}

	// dolne prawe
	if (level->board[currentY + 1][currentX + 1].type == Level::NONE || level->board[currentY + 1][currentX + 1].type == Level::GRASS)
	{
		level->board[currentY + 1][currentX + 1].type = Level::NONE;

		if (playerX == currentX + 1 && playerY == currentY + 1)
		{
			player.setIsDead(true);
		}
	}

	isExplosion = true; // ustawienie pola informujacego o tym ze dany obiekt wybuchl i moze byc usuniety przez funkcje clearUsedFireStone w klasie Engine
}