#include "Game.h"


void Game::runGame()
{
	while (state != END)
	{
		switch (state)
		{
		case MENU:
			menu();
			break;
		case GAME:
			play();
			state = MENU;
			break;
		case SCORES:
			viewBestScores();
			state = MENU;
			break;
		}
	}
}

void Game::menu()
{	
	sf::Texture obrazek;
	obrazek.loadFromFile("background.png");
	sf::Sprite sprite;
	sprite.setTexture(obrazek);
	sprite.setPosition(0, 0);
	
	setTitle();
	setButtons();

	while (state == MENU)
	{
		checkClick();
		window.clear();
		window.draw(sprite);
		window.draw(title);
		
		for (int i = 0; i < NUMBER; i++)
		{
			window.draw(buttons[i]);
		}

		window.display();
	}			
}

void Game::setTitle()
{
	const int SIZE = 80;
	title.setCharacterSize(SIZE);
	title.setFont(font);
	title.setString("Boulder Dash");
	title.setPosition(1280 / 2 - title.getGlobalBounds().width / 2, 20);
}

void Game::setButtons()
{
	string str[] = { "Start","Best Scores", "Exit" };
	for (int i = 0; i < NUMBER; i++)
	{
		buttons[i].setFont(font);
		buttons[i].setCharacterSize(65);
		buttons[i].setString(str[i]);
		buttons[i].setPosition(1280 / 2 - buttons[i].getGlobalBounds().width / 2, 250 + i * 120);
	}
}

void Game::checkClick()
{
	sf::Vector2i mouse_position(sf::Mouse::getPosition(window));	//	pobranie wspolrzednych myszy
	sf::Vector2f mouse = window.mapPixelToCoords(mouse_position);	// wspolrzedne myszy niezalezne od zmiany rozmiaru okna

	sf::Event event;

	setButtonsLight(mouse);	// podswietlenie przycisku jesli znajduje sie na nim kursor

	while (window.pollEvent(event))
	{
		// wcisniecie x - zamkniecie okna aplikacji
		if (event.type == sf::Event::Closed)
			state = END;

		//klikniecie START                                        
		else if (buttons[START].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
		{
			state = GAME;
		}

		//klikniecie BEST_SCORES
		else if (buttons[BEST_SCORES].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
		{
			state = SCORES;
		}
		//klikniecie EXIT
		else if (buttons[EXIT].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
		{
			state = END;
		}
	}
}

void Game::setButtonsLight(sf::Vector2f mouse)
{
	for (int i = 0; i < NUMBER; i++)
		if (buttons[i].getGlobalBounds().contains(mouse))
			buttons[i].setColor(sf::Color::Cyan);
		else buttons[i].setColor(sf::Color::White);
}

void Game::play()
{
	Engine engine(window); // stworzenie instancji obiektu odpowiedzialnego za rozgrywke
	state = MENU;	// po zakonczonej grze powrot do menu
}

void Game::viewBestScores()
{
	vector<PlayerData*> players;	// vector sluzacy do przechowywania danych graczy wczytanych z pliku
	vector<sf::Text*> playersString; // vector przechowuje stringi do wyswietlenia z najlepszymi graczami
	
	loadScoresFromFile(players);

	const int SIZE = 50;	
	setTextObject(title, SIZE, 360, 50, font, "Best scores");
	
	int space = 150; // odstep pomiedzy wyswietlanymi wynikami a tytulem

	for (int i = 0; i < players.size(); ++i, space += 100) // petla kopiuje vector players do wektora playersString i formatuje odpowiednio dane do wyswietlenia
	{
		playersString.push_back(new sf::Text());
		setTextObject(*playersString[i], SIZE, 360, space, font);

		ostringstream str;
		str << i + 1 << ". " << players[i]->getName() << " " << players[i]->getScore(); // stworzenie jednego stringu z imieniem gracza i wynikiem
		playersString[i]->setString(str.str());
	}
	
	window.clear();

	for_each(playersString.begin(), playersString.end(), [&](sf::Text* player) {	// rysowanie w oknie najlepszych wynikow
		window.draw(*player);
	});

	window.draw(title);
	window.display();
	
	sf::Event event;
	bool end = false;

	while (!end) // petla wyswietlajaca wyniki do czasu wcisniecia Escape
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				end = true;
				
				for (auto i = players.begin(); i != players.end(); ++i)
				{
					delete *i;
				}
				
				for (auto i = playersString.begin(); i != playersString.end(); ++i)
				{
					delete *i;
				}
			}
		}
	}

}

sf::Font Game::loadFont(string font)
{
	sf::Font tmpFont;
	if (!tmpFont.loadFromFile(font))
	{
		throw exception();
	}
	return tmpFont;
}

void setTextObject(sf::Text& text, int size, int x, int y, sf::Font& font, string value)
{
	text.setCharacterSize(size);
	text.setFont(font);
	text.setString(value);
	text.setPosition(x, y);
}

void loadScoresFromFile(vector<PlayerData*>& players)
{
	fstream file;
	file.open("bestScores.txt", ios::in);
	string name;
	string tmpScore;
	int score;

	file >> name;
	while (!file.eof())
	{
		file >> tmpScore;
		score = atoi(tmpScore.c_str());
		players.push_back(new PlayerData(name, score));
		file >> name;
	}

	file.close();
}