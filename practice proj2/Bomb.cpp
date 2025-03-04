#include "Bomb.h"

Bomb::Bomb(int i, int j, int playerNumber, Clock clock, Texture& bomb_t)
{
	playerNo = playerNumber;
	x = j;
	y = i;
	plantTime = clock.getElapsedTime().asSeconds();
	rect.setSize(Vector2f(50, 50));
	rect.setFillColor(Color::Green);
	rect.setPosition(Vector2f(x * 50, y * 50));
	setTexture(bomb_t);
}

Bomb::~Bomb()
{
}

//Checks if enough time has passed since bomb was planted//
bool Bomb::update(Clock clock)
{
	if (clock.getElapsedTime().asSeconds() >= plantTime + 3 && clock.getElapsedTime().asSeconds() <= plantTime + 3.2)		//checks if 3 seconds have passed
	{
		std::cout << "Bomb exploded\n";
		return true;
	}
	else
		return false;
}

//sets texture and sprite for bomb//
void Bomb::setTexture(Texture& bomb_t)
{
	sprite.setTexture(bomb_t);
	sprite.setScale(Vector2f(0.183, 0.1915));
	sprite.setPosition(Vector2f(50 * x, 50 * y));
}
