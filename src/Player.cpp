#include "Player.h"

Player::Player(sf::RenderWindow& window)
{
	if (!texture.loadFromFile("player.png"))
	{
		sf::Text errorMsg;
		sf::Font font;
		font.loadFromFile("Guardians.ttf");
		errorMsg.setFont(font);
		errorMsg.setCharacterSize(30);
		errorMsg.setPosition(0, 0);
		errorMsg.setString("Some texture can not be loaded.\nProgram will stop");
		window.clear();
		window.draw(errorMsg);
		window.display();
		sf::sleep(sf::seconds(3));
		exit(EXIT_FAILURE);
	}

	sprite.setTexture(texture);
	status = STOP;
	sprite.setPosition(352, 352);
	isDead = false;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite);
}

void Player::update(int direction)
{
	if (anim_clock.getElapsedTime() > sf::seconds(SPEED))
	{
		if (status == STOP)
		{
			return;
		}

		sprite.move(displacement(direction));
		anim_clock.restart();
	}
}

void Player::stop()
{
	status = STOP;
}

void Player::go()
{
	status = GO;
}

sf::Vector2f Player::getPosition()
{
	return sprite.getPosition();
}

Player::Status Player::getStatus()
{
	return status;
}

sf::FloatRect Player::getBoundingBox()
{
	return sprite.getGlobalBounds();
}

sf::Vector2f Player::displacement(int direction)
{
	int x = 0;
	int y = 0;

	switch (direction)
	{
	case UP:
		x = 0;
		y = -Level::TILE_HEIGHT;
		break;
	case DOWN:
		x = 0;
		y = Level::TILE_HEIGHT;
		break;
	case RIGHT:
		x = Level::TILE_WIDTH;
		y = 0;
		break;
	case LEFT:
		x = -Level::TILE_WIDTH;
		y = 0;
		break;
	}

	return sf::Vector2f(x, y);
}