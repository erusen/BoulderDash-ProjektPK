#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include <iostream>

class Level
{
public:
	void loadFromFile(std::string filename);

	enum FieldType {	// typy obiektow (kafelek) w grze
		BRICK,
		GRASS,
		STONE,
		DIAMOND,
		CLONING_STONE,
		FRAGILE_DIAMOND,
		DIAMOND_IN_SHELL,
		DIAMOND_WITHOUT_SHELL,
		FIRE_STONE,
		NONE
	};

	struct Tile	// zagniezdzona struktura przechowujaca informacje o typie danego kafelka i czy jest bariera dla gracza
	{
		FieldType type;
		bool isWall;
	};

	// ilosc kafelkow
	static const int WIDTH = 40;
	static const int HEIGHT = 20;

	// rozmiar kafelka
	static const int TILE_WIDTH = 32;
	static const int TILE_HEIGHT = 32;

	Tile board[HEIGHT][WIDTH]; // plansza gry
};

#endif