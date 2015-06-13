#include "Level.h"

using std::fstream;
using std::cout;
using std::string;

void Level::loadFromFile(string filename)
{
	fstream file;
	file.open(filename, std::ios::in);

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int tmp;
			char ts;
			file >> ts;
			tmp = ts - 48;	// - kod ASCII
			board[y][x].type = FieldType(tmp);

			if (tmp == BRICK || tmp == STONE || tmp == CLONING_STONE || tmp == DIAMOND_IN_SHELL || tmp == FIRE_STONE)
			{
				board[y][x].isWall = true;
			}
			else
			{
				board[y][x].isWall = false;
			}
		}
	}

	file.close();
}