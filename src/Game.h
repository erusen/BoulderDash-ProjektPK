#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Engine.h"
#include "PlayerData.h"

using std::string;
using std::exception;
using std::vector;

void loadScoresFromFile(vector<PlayerData*>& players); // laduje dane z pliku i umieszcza je jako obiekty PlayerData w przekazanym wektorze
void setTextObject(sf::Text& text, int size, int x, int y, sf::Font& font, string value = ""); // ustawia wszystkie potrzebne pola obiektu klasy sf::Text

class Game
{
	enum GameState { MENU, GAME, END, SCORES};
	enum Buttons { START, BEST_SCORES, EXIT };
	GameState state;	// okresla stan gry czy wyswietlac menu, uruchomic rozgrywke, zakonczyc gre, wyswietlic najlepsze wyniki
	sf::RenderWindow window;	// okno gry
	sf::Font font;
	sf::Text title;	// tytul gry wyswietlany w glownym menu
	static const int NUMBER = 3;	// ilosc przyciskow w menu
	sf::Text buttons[NUMBER];	// 
	void menu(); // metoda odpowiedzialna za rysowanie i obsluge menu
	void play();	//  tworzy obiekt klasy engine, uruchamienie gry
	sf::Font loadFont(string font);
	void setTitle();	// ustawia czcionke, rozmiar, tekst, pozycje wyswietlanego w menu tytulu gry
	void setButtons();	// ustawia czcionke, rozmiar, tekst, pozycje wyswietlanych przyciskow
	void setButtonsLight(sf::Vector2f mouse);	// ustawia podswietlenie przycisku po najechaniu myszka
	void checkClick();	// sprawdza czy ktorys z przyciskow zostal wybrany
	void viewBestScores();	// wyswietla liste najlepszych graczy

public:
	Game() : window(sf::VideoMode(1280, 720), "Boulder Dash") // konstruktor inicjalizuje okno gry (ustala rozmiar i tytul okna)
	{
		font = loadFont("Guardians.ttf");
		state = MENU; // ustawienie poczatkowego stanu programu na wyswietlenie menu
	}

	void runGame(); // metoda odpowiedzialna za zwywolywanie poszczegolnych modulow programu glownego menu, rozgrywki, najlepszych wynikow
};

#endif