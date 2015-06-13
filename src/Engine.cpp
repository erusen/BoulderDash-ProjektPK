#include "Engine.h"

Engine::Engine(sf::RenderWindow& window) : player(window)
{
	scores = 0;		// ustawienie stanu punktow na 0
	state = GAME; // ustawienie stanu gry na trwajaca rozgrywke
	loadTexture(window); //wczytanie tekstur	
	setLevel(); //ustawienie kafli
	setHUD(); // ustawienie informacji dla gracza

	runEngine(window); // wywolanie metody zawierajacej glowna petle gry i zawierajaca logike
}

void Engine::check_collision(int direction)
{
	if (player.getStatus() == Player::STOP) // jesli gracz aktualnie stoi to nie sprawdzaj kolizji
	{
		return;
	}

	sf::FloatRect playerBox(player.getBoundingBox()); //pobranie pozycji gracza

	// aktualizacja pozycji gracza do sytuacji gdyby wykonal ruch
	playerBox.left += player.displacement(direction).x;
	playerBox.top += player.displacement(direction).y;

	int x = (playerBox.left + (level.TILE_WIDTH / 2)) / level.TILE_WIDTH;
	int y = (playerBox.top + (level.TILE_HEIGHT / 2)) / level.TILE_HEIGHT;
	
	if (level.board[y][x].isWall)
	{
		player.stop();
	}
}

void Engine::runEngine(sf::RenderWindow &window)
{
	sf::Event event;
	bool menu = false; // zmienna odpowiedzialna za powrot do menu
	const int FRAMES_PER_SECOND = 25;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	int ticks = clock();
	double sleep_time = 0;

	while (!menu) // petla glowna rozgrywki
	{
		window.pollEvent(event); // przechwytywanie zdarzen, nieprzechwycone zdarzenia powoduja zawieszenie programu
		
		int direction; // kierunek ruchu gracza

		moveObjectOnBoard();

		checkIsKeyPressed(direction, player, menu); // odczyt z klawiatury kierunku ruchu lub wyjscia z gry
		check_collision(direction); // sprawdzenie kolizji gracza ze scianiami
		player.update(direction); // aktualizacja pozycji gracza
		hideGrass();

		collectDiamonds();
		clearUsedFireStone();
		cloneStones();

		window.clear();

		drawLevel(window);

		if (isEnd(state, window))		// sprawdzenie konca gry
		{
			menu = true;
		}
		else	// gra nadal trwa 
		{
			window.draw(player);
			window.draw(diamondsOnBoard);
			window.draw(scoresBaner);
			window.display();

			// opoznienie gry zeby na roznych komputerach gra dzialala z jednakowa predkoscia
			ticks += SKIP_TICKS;
			sleep_time = ticks - clock();
			if (sleep_time >= 0)
			{
				sf::sleep(sf::seconds(sleep_time / CLOCKS_PER_SEC));
			}			
		}
		
	}

	Diamond::setDiamondsNumber(0); // wyzerowanie licznika zebranych diamentow dla kolejnej rozgrywki (zmienna statyczna)
}

bool Engine::moveStone(int direction)
{
	int dX;
	int x = 0;
	int y = 0;	

	sf::FloatRect PlayerBox(player.getBoundingBox()); // pozycja gracza
	
	PlayerBox.left += (player.displacement(direction).x); // obliczenie nowej pozycj gracza po przesunieciu
	PlayerBox.top += (player.displacement(direction).y);

	x = (PlayerBox.left) / level.TILE_WIDTH; // obliczenie wspolrzednych gracza
	y = PlayerBox.top / level.TILE_HEIGHT;

	if (direction == 90)	// przesuwanie kamienia w prawo
	{
		dX = 1; // kolejna kratka, ruch w prawo
	}

	else if (direction == -90)	// przesuwanie kamienia w lewo
	{
		dX = -1; // poprzednia kratka ruch w lewo
	}

	// sprawdzenie czy obiekt ktory chcemy przesunac jest typu STONE lub FIRE_STONE i czy nastepne za nim pole jest wolne
	if ((level.board[y][x].type == Level::STONE || level.board[y][x].type == Level::FIRE_STONE) && (level.board[y][x + dX].type == Level::NONE))
	{
		for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end(); ++iter) // szukanie obiektu do przesuniecia ktory ma takie same wspolrzedne jak gracz
		{
			if ((*iter)->getX() == x && (*iter)->getY() == y) // sprawdzenie czy obiekt ma takie same wspolrzedne jak gracz
			{
				if (level.board[y][x].type == Level::STONE) // jesli obiekt to Stone to aktualizujemy nowe pole kamienia po przesunieciu
				{
					level.board[y][x + dX].type = Level::STONE; // nadanie odpowiedniego typu nowemu polu
					sprite[y][x + dX].setTexture(texture[Level::STONE]); // aktualizacja tekstury nowego pola
				}
				else if (level.board[y][x].type == Level::FIRE_STONE) // jesli obiekt to FireStone to aktualizujemy nowe pole kamienia po przesunieciu
				{
					level.board[y][x + dX].type = Level::FIRE_STONE;
					sprite[y][x + dX].setTexture(texture[Level::FIRE_STONE]);
				}

				player.update(direction); // aktualizacja nowej pozycji gracza
				level.board[y][x].type = Level::NONE; // ustawienie poprzedniej pozycji kamienia jako wolne pole
				level.board[y][x].isWall = false;		
				
				// aktualizacja nowego pola zajetego przez kamien i wspolrzednych obiektu
				level.board[y][x + dX].isWall = true;
				sprite[y][x + dX].setPosition((x + dX)*Level::TILE_WIDTH, y*Level::TILE_HEIGHT);
				(*iter)->setX(x + dX);
				(*iter)->setY(y);

				sf::sleep(sf::seconds(0.2)); // spowolnienie przesuniecia kamienia
				return true;
			}
		}
	}
	return false;
}

bool Engine::isEnd(State state, sf::RenderWindow &window)
{
	if (Diamond::getDiamondsNumber() == 0) // zebrano wszystkie diamenty
	{
		state = WIN;
	}

	if (state == WIN || state == LOSS || player.getIsDead() == true)
	{
		string result = (state == WIN) ? "You win!" : "You lost!";

		sf::Text endText;
		const int SIZE = 100;
		endText.setCharacterSize(SIZE);
		endText.setFont(font);
		endText.setString(result);
		endText.setPosition(300, 150);

		window.draw(endText);
		window.draw(scoresBaner);
		window.draw(diamondsOnBoard);
		window.display();
		sf::sleep(sf::seconds(2));
		Diamond::setDiamondsNumber(0);
		checkBestScores(window);
		return true;
	}

	return false;
}

void Engine::checkIsKeyPressed(int& direction, Player& player, bool& menu)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		menu = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		player.go();
		direction = Player::UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		player.go();
		direction = Player::DOWN;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		player.go();
		direction = Player::LEFT;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			moveStone(direction);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		player.go();
		direction = Player::RIGHT;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			moveStone(direction);
		}
	}
	if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
	{
		player.stop();
	}
}

void Engine::collectDiamonds()
{
	sf::FloatRect box(player.getBoundingBox()); // pobranie pozycji gracza
	int x = (box.left + (level.TILE_WIDTH / 2)) / level.TILE_WIDTH; // obliczenie wspolrzednych srodka gracza
	int y = (box.top + (level.TILE_HEIGHT / 2)) / level.TILE_HEIGHT;

	for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end(); ++iter)	// sprawdzenie czy jakis diament zostal zebrany
	{
		if ((*iter)->getX() == x && (*iter)->getY() == y) // sprawdzenie czy jakis diament ma takie same wspolrzedne jak gracz
		{
			string type = typeid(**iter).name();
			if (type == "class Diamond" || type == "class FragileDiamond" || (type == "class DiamondInShell" && level.board[y][x].isWall == false)) // spradzenie czy obiekt jest diamentem
			{
				scores += 100;	// dodanie punktow
				delete *iter;
				objectOnBoard.remove(*iter); // usuniecie diamentu z listy obiektow na planszy
				level.board[y][x].type = Level::NONE; // ustawienie pola po diamencie jako wolne
				
				// aktualizacja HUD
				Diamond::setDiamondsNumber(Diamond::getDiamondsNumber() - 1);
				ostringstream sd;
				sd << Diamond::getDiamondsNumber();
				diamondsOnBoard.setString("Diamonds " + sd.str());

				ostringstream ss;
				ss << scores;
				scoresBaner.setString("Scores " + ss.str());
				break;
			}
		}
	}
}

void Engine::clearUsedFireStone()
{
	for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end();) // odszukanie na planszy obiektow klasy FireStone
	{
		string fireStoneType = "class FireStone";

		if (typeid(**iter).name() == fireStoneType && ((FireStone*)(*iter))->getIsExplosion() == true) // sprawdzenie czy dany obiekt to FireStone i czy wybuchl
		{
			// jesli wybuchl to dane pole zostaje ustawione jako wolne a obiekt jest usuwany z listy
			level.board[(*iter)->getY()][(*iter)->getX()].isWall = false;
			level.board[(*iter)->getY()][(*iter)->getX()].type = Level::NONE;
			delete *iter;
			objectOnBoard.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}

void Engine::cloneStones()
{
	for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end();) // szukanie obiektow klasy CloningStone
	{
		string cloningStoneType = "class CloningStone";

		if (typeid(**iter).name() == cloningStoneType && ((CloningStone*)(*iter))->getIsCloning() == true) // sprawdzenie czy dany obiekt to CloningStone i czy ma stworzyc nowe kamienie
		{
			// jesli maja byc stworzone nowe kamienie to aktualne pole obiektu ustawiane jest jako wolne
			level.board[(*iter)->getY()][(*iter)->getX()].isWall = false;
			level.board[(*iter)->getY()][(*iter)->getX()].type = Level::NONE;
			
			// sprawdzenie wszystkich sasiadujacych pol z danym obiektem, jesli dane pole jest typu STONE to zostanie tam stworzony nowy kamien
			// up left
			if (level.board[(*iter)->getY() - 1][(*iter)->getX() - 1].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX() - 1, (*iter)->getY() - 1));
			}

			// up center
			if (level.board[(*iter)->getY() - 1][(*iter)->getX()].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX(), (*iter)->getY() - 1));
			}

			// up right
			if (level.board[(*iter)->getY() - 1][(*iter)->getX() + 1].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX() + 1, (*iter)->getY() - 1));
			}

			// middle left
			if (level.board[(*iter)->getY()][(*iter)->getX() - 1].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX() - 1, (*iter)->getY()));
			}

			// middle right
			if (level.board[(*iter)->getY()][(*iter)->getX() + 1].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX() + 1, (*iter)->getY()));
			}

			// down left
			if (level.board[(*iter)->getY() + 1][(*iter)->getX() - 1].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX() - 1, (*iter)->getY() + 1));
			}

			// down center
			if (level.board[(*iter)->getY() + 1][(*iter)->getX()].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX(), (*iter)->getY() + 1));
			}

			// down right
			if (level.board[(*iter)->getY() + 1][(*iter)->getX() + 1].type == Level::STONE)
			{
				objectOnBoard.push_back(new Stone((*iter)->getX() + 1, (*iter)->getY() + 1));
			}

			delete *iter;
			objectOnBoard.erase(iter++); // usuniecie obiektu CloningStone po stowrzeniu nowych kamieni
		}
		else
		{
			++iter;
		}
	}
}

void Engine::checkBestScores(sf::RenderWindow& window)
{
	vector<PlayerData*> players;

	loadScoresFromFile(players);

	auto minIter = min_element(players.begin(), players.end(), [](PlayerData* p1, PlayerData* p2) { return p1->getScore() < p2->getScore(); }); // odszukanie gracza z najmniejsza iloscia punktow
	int minScore = ((PlayerData*)(*minIter))->getScore(); // zapisanie najmniejszego wyniku

	if (scores <= minScore) // jesli wynik gracza jest mniejszy od najnizszego najlepszego wyniku to wyjdz bez zapisywania uzyskanego wyniku
	{
		for (auto i = players.begin(); i != players.end(); ++i)
		{
			delete *i;
		}
		return;
	}

	sf::Text text;
	sf::Text title;
	const int SIZE = 70;
	const int MAX_NICK_LENGTH = 10;
	const int MAX_BEST_PLAYERS = 5;
	
	setTextObject(text, SIZE, 300, 300, font);
	setTextObject(title, SIZE, 230, 50, font, "Give your nick");
	
	string str;
	sf::Event event;
	bool end = false;
	while (window.pollEvent(event));

	while (!end)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::TextEntered)
			{
				if (str.length() <= MAX_NICK_LENGTH) // sprawdzenie czy dlugosc nicku gracza nie przekracza dozwolonej wartosci
				{
					checkLetter(event, str); // sprawdzenie kodu UNICODE wpisanego znaku
					text.setString(str);
				}		
			}
			if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::BackSpace) // usuwanie wpisanych znakow
			{
				if (str.length() > 0) // sprawdzenie czy string zawiera jakies znaki
				{
					str.erase(str.length() - 1, 1);
				}
			}
			if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::Return) // wcisniecie Enter konczy wpisywanie
			{
				end = true;
			}
		}
		window.clear();
		window.draw(title);
		window.draw(text);
		window.display();
	}
	
	players.push_back(new PlayerData(str, scores)); // dodanie wyniku gracza do wektora z najlepszymi wynikami

	sort(players.begin(), players.end(), [](PlayerData* p1, PlayerData* p2) { return p1->getScore() > p2->getScore(); }); // posortowanie wynikow w kolejnosci malejacej
	

	if (players.size() > MAX_BEST_PLAYERS) // jesli liczba graczy jest wieksza od maksymalnej dlugosci listy najlepszych wynikow to usun najgorszych graczy
	{
		for (int i = 0; i < players.size(); ++i)
		{
			if (i >= MAX_BEST_PLAYERS)
			{
				delete players[i];
				players.erase(players.begin() + i);
				--i;
			}
		}
	}
	
	saveScoresToFile(players);	

	for (auto i = players.begin(); i != players.end(); ++i)
	{
		delete *i;
	}
}

void Engine::loadTexture(sf::RenderWindow& window)
{
	bool error = false;
	if(!texture[Level::BRICK].loadFromFile("brick.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if(!texture[Level::GRASS].loadFromFile("grass.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if(!texture[Level::STONE].loadFromFile("stone.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if (!texture[Level::DIAMOND].loadFromFile("diamond.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if (!texture[Level::CLONING_STONE].loadFromFile("cloningStone.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if (!texture[Level::FRAGILE_DIAMOND].loadFromFile("fragileDiamond.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if (!texture[Level::DIAMOND_IN_SHELL].loadFromFile("diamondInShell.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if (!texture[Level::DIAMOND_WITHOUT_SHELL].loadFromFile("diamondWithoutShell.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true;
	else if (!texture[Level::FIRE_STONE].loadFromFile("fireStone.png", sf::IntRect(0 * level.TILE_WIDTH, 0, level.TILE_WIDTH, level.TILE_HEIGHT)))
		error = true; 
	
	if (error)
	{
		sf::Text errorMsg; 
		font.loadFromFile("Guardians.ttf");
		setTextObject(errorMsg, 30, 0, 0, font, "Some texture can not be loaded.\nProgram will stop");
		window.clear();
		window.draw(errorMsg);
		window.display();
		sf::sleep(sf::seconds(3));
		this->~Engine();
		exit(EXIT_FAILURE);
	}
}

void Engine::setLevel()
{
	level.loadFromFile("level.txt");
	for (int i = 0; i<level.HEIGHT; i++)
	{
		for (int j = 0; j<level.WIDTH; j++)
		{
			sprite[i][j].setTexture(texture[level.board[i][j].type]);
			sprite[i][j].setPosition(j*level.TILE_WIDTH, i*level.TILE_HEIGHT);


			switch (level.board[i][j].type)	// stworzenie kamieni i diamentow
			{
			case Level::STONE:
				objectOnBoard.push_back(new Stone(j, i));	
				break;
			case Level::DIAMOND:
				objectOnBoard.push_back(new Diamond(j, i));	
				break;
			case Level::FRAGILE_DIAMOND:
				objectOnBoard.push_back(new FragileDiamond(j, i));
				break;
			case Level::DIAMOND_IN_SHELL:
				objectOnBoard.push_back(new DiamondInShell(j, i));
				break;
			case Level::FIRE_STONE:
				objectOnBoard.push_back(new FireStone(j, i));
				break;
			case Level::CLONING_STONE:
				objectOnBoard.push_back(new CloningStone(j, i));	
				break;
			}
		}
	}
}

void Engine::setHUD()
{
	const int SIZE = 50;
	font.loadFromFile("Guardians.ttf");

	diamondsOnBoard.setCharacterSize(SIZE);
	diamondsOnBoard.setFont(font);
	ostringstream sd;
	sd << Diamond::getDiamondsNumber();
	diamondsOnBoard.setString("Diamonds " + sd.str());
	diamondsOnBoard.setPosition(750, 650);

	scoresBaner.setCharacterSize(SIZE);
	scoresBaner.setFont(font);
	ostringstream ss;
	ss << scores;
	scoresBaner.setString("Scores " + ss.str());
	scoresBaner.setPosition(20, 650);
}

void Engine::checkLetter(sf::Event& event, string& str)
{
	switch (event.text.unicode)
	{
	case 'a':
	case 'A':
		str += "a";
		break;
	case 'b':
	case 'B':
		str += "b";
		break;
	case 'c':
	case 'C':
		str += "c";
		break;
	case 'd':
	case 'D':
		str += "d";
		break;
	case 'e':
	case 'E':
		str += "e";
		break;
	case 'f':
	case 'F':
		str += "f";
		break;
	case 'g':
	case 'G':
		str += "g";
		break;
	case 'h':
	case 'H':
		str += "h";
		break;
	case 'i':
	case 'I':
		str += "i";
		break;
	case 'j':
	case 'J':
		str += "j";
		break;
	case 'k':
	case 'K':
		str += "k";
		break;
	case 'l':
	case 'L':
		str += "l";
		break;
	case 'm':
	case 'M':
		str += "m";
		break;
	case 'n':
	case 'N':
		str += "n";
		break;
	case 'o':
	case 'O':
		str += "o";
		break;
	case 'p':
	case 'P':
		str += "p";
		break;
	case 'q':
	case 'Q':
		str += "q";
		break;
	case 'r':
	case 'R':
		str += "r";
		break;
	case 's':
	case 'S':
		str += "s";
		break;
	case 't':
	case 'T':
		str += "t";
		break;
	case 'u':
	case 'U':
		str += "u";
		break;
	case 'v':
	case 'V':
		str += "v";
		break;
	case 'w':
	case 'W':
		str += "w";
		break;
	case 'x':
	case 'X':
		str += "x";
		break;
	case 'y':
	case 'Y':
		str += "y";
		break;
	case 'z':
	case 'Z':
		str += "z";
		break;
	}
}

void Engine::hideGrass()
{
	int x = player.getPosition().x / level.TILE_WIDTH;
	int y = player.getPosition().y / level.TILE_HEIGHT;
	
	if (level.board[y][x].type = Level::GRASS)
	{
		level.board[y][x].type = Level::NONE;
	}
}

void Engine::moveObjectOnBoard()
{
	for (auto iter = objectOnBoard.begin(); iter != objectOnBoard.end(); ++iter) // spadanie kamieni i diamentow
	{
		(*iter)->moveDown(&level, sprite, texture, player);
		string type = typeid(**iter).name();
		if (type == "class FragileDiamond")	// sprawdzenie czy obiekt jest klasy FragileDiamond
		{
			if (!(((FragileDiamond*)(*iter))->breakDiamond(objectOnBoard, player, sprite))) // sprawdzenie czy FragileDiamond sie rozbil
			{
				state = LOSS; // FragileDiamond sie rozbil, koniec gry
			}
		}
		if (type == "class DiamondInShell") // sprawdzenie czy obiekt jest klasy DiamondInShell
		{
			((DiamondInShell*)(*iter))->breakShell(objectOnBoard, &level, sprite, texture); // sprawdzenie czy muszla zostala rozbita
		}
	}
}

void Engine::drawLevel(sf::RenderWindow& window)
{
	for (int i = 0; i < level.HEIGHT; i++)
	{
		for (int j = 0; j < level.WIDTH; j++)
			if (level.board[i][j].type != Level::NONE)
				window.draw(sprite[i][j]);
	}
}

void Engine::saveScoresToFile(vector<PlayerData*> players)
{
	fstream file;
	file.open("bestScores.txt", ios::out);

	for_each(players.begin(), players.end(), [&](PlayerData* player) {  // zapis najlepszych graczy do pliku
		file << player->getName();
		file << " ";
		file << player->getScore();
		file << "\n";
	});

	file.close();
}

Engine::~Engine()
{
	for (auto i = objectOnBoard.begin(); i != objectOnBoard.end(); ++i)
	{
		delete *i;
	}
}