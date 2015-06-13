#ifndef ENGINE_H
#define ENGINE_H

#include <SFML\Graphics.hpp>
#include "Level.h"
#include "Player.h"
#include "ObjectOnBoard.h"
#include "Diamond.h"
#include "FragileDiamond.h"
#include "DiamondInShell.h"
#include "Stone.h"
#include "FireStone.h"
#include "CloningStone.h"
#include <string>
#include <sstream>
#include <fstream>
#include "PlayerData.h"
#include <algorithm>
#include <iterator>
#include <list>
#include "Game.h"

class Engine
{
public:
	Engine(sf::RenderWindow &window);
	~Engine();
	enum State { GAME, WIN, LOSS };
	State state;
	void check_collision(int direction); // wykrywanie kolizji gracza z scianami
	void runEngine(sf::RenderWindow &window); // metoda zawierajaca petle glowna gry i logike
	
private:
	sf::Texture texture[9]; // tablica przechowujaca wszystkie tekstury
	sf::Sprite sprite[Level::HEIGHT][Level::WIDTH]; // tablica przechowujaca rozmieszczenie tekstur na planszy
	Level level; // obiekt przechowujacy informacje o rozmieszczeniu obiektow na planszy
	Player player;
	list<ObjectOnBoard*> objectOnBoard; // lista przechowujaca wszystkie diamenty i kamienie na planszy
	sf::Text diamondsOnBoard; // napis pokazujacy liczbe pozostalych na planszy diamentow
	sf::Text scoresBaner;	// napis pokazujacy liczbe zebranych punktow
	int scores; // zmienna przechowuje ilosc zebranych puntow
	sf::Font font;

	bool moveStone(int direction); // metoda odpowiedzialna za przesuwanie kamieni na boki przez gracza
	bool isEnd(State state, sf::RenderWindow &window); // metoda sprawdzajaca czy gra sie zakonczyla
	void checkIsKeyPressed(int& direction, Player& player, bool& menu); // odczyt z klawiatury ruchow gracza
	void collectDiamonds();	// sprawdzenie czy jakis diament zostal zebrany i usuniecie go z listy obiektow na planszy
	void clearUsedFireStone(); // usuniecie z listy obiektow na planszy kamieni ktore juz wybuchnely
	void cloneStones();	// dodaje do listy obiektow nowe kamienie i usuwa kamien ktory stworzyl nowe obiekty
	void checkBestScores(sf::RenderWindow& window); // sprawdzenie czy gracz moze dopisac sie do listy najlepszych wynikow
	void loadTexture(sf::RenderWindow& window); // metoda odpowiedzialna za zaladowanie tekstur
	void setLevel(); // poczatkowe ustawienie levelu
	void setHUD(); // ustawienie wyswietlania informacji o stanie diamentow i punktow
	void checkLetter(sf::Event& event, string& str); // rozpoznawanie UNICODE wprowadzanych liter podczas podawania nazwy gracza
	void hideGrass(); // metoda odpowiedzialna za znikanie trawy na polu na ktorym znajduje sie gracz
	void moveObjectOnBoard(); // metoda przechodzi po wszystkich obiektach na planszy i jesli to potrzebne przesuwa je
	void drawLevel(sf::RenderWindow& window); // rysuje board i wszystkie obiekty na planszy
	void saveScoresToFile(vector<PlayerData*> players); // zapisuje najlepsze wyniki do pliku
};

#endif