#ifndef PLAYER_H
#define PLAYER_H

#include <SFML\Graphics.hpp>
#include "Level.h"

class Player : public sf::Drawable, sf::Transformable
{
public:
	enum Status { GO, STOP };
	const double SPEED = 0.2;
	static const int UP = 0;
	static const int DOWN = 180;
	static const int RIGHT = 90;
	static const int LEFT = -90;

	bool getIsDead() { return isDead; }
	void setIsDead(bool isDead) { this->isDead = isDead; }
	Player(sf::RenderWindow& window);
	sf::Vector2f getPosition();
	Status getStatus();
	sf::FloatRect getBoundingBox();
	sf::Vector2f displacement(int direction);
	void update(int direction); // przesuwanie gracza w zadanym kierunku
	void stop(); // zmiana statusu na STOP
	void go();	// zmiana statusu na GO
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
private:
	sf::Sprite sprite;
	sf::Texture texture;
	Status status;	// informacja czy gracz idzie czy stoi
	bool isDead;	// informacja czy gracz jest zywy czy martwy
	sf::Clock anim_clock;
};

#endif