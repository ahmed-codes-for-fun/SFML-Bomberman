#include "mapSpace.h"

//constructor//
mapSpace::mapSpace()
{
	player1 = false;
	player2 = false;
	enemy = false;
	explosion = false;
	wall = false;
	breakabaleWall = false;
	bomb = false;
	rect.setSize(Vector2f(0, 0));
}


void mapSpace::setWall(int i, int j, bool breakable, Texture& break_t,Texture& wall_t)
{
	if (!breakable)
	{
		sprite.setTexture(wall_t);	//setting texture for unbreakable wall
		sprite.setScale(Vector2f(0.17921, 0.1831));		//adjusting size
	}
	else
	{
		sprite.setTexture(break_t);		//setting texture for breakable wall
		sprite.setScale(Vector2f(0.222, 0.222));	//adjusting size
	}
	//setting position in pixels
	sprite.setPosition(Vector2f(j * 50, i * 50));
}

//Sets Explosions//
void mapSpace::setExplosion(int i, int j, Clock clock, Texture& explosion_t, Player* player_1, Player* player_2, Game& gameObj)
{
	explosion_s.setTexture(explosion_t);
	explosion_s.setScale(Vector2f(0.1677, 0.1886));
	explosion_s.setPosition(Vector2f(j * 50, i * 50));

	explodeTime = clock.getElapsedTime().asSeconds();
	if (breakabaleWall)
	{
		breakabaleWall = false;
	}
	
	
}

void mapSpace::removeExplosion()
{
	explosion_s.setScale(Vector2f(0, 0));
	explosion = false;
}

