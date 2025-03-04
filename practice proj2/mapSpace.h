#pragma once
#include "Game.h"
using namespace sf;
class Player;
class Game;

//Defines each tile on the map of the game, having all details of what is present on the tile and a few methods//
class mapSpace
{
public:
	//flags to show if the particular tile has a wall or player 1 present or an enemy present or a bomb//
	bool breakabaleWall, player1, player2, enemy, bomb, explosion, wall;	
	
	//Time for the explosions set on the tile//
	float explodeTime;

	//Rectangle was temporary before adding graphics//
	RectangleShape rect;

	//Sprite for breakable or unbreakable wall//
	Sprite sprite;

	//Sprite for Explosion//
	Sprite explosion_s;

	//functions//
	mapSpace();
	void setWall(int i, int j, bool breakable, Texture& break_t, Texture& wall_t);	//sets a wall on the tile
	void setExplosion(int i, int j, Clock clock, Texture& explosion_t, Player* player1, Player* player2, Game& gameObj);	//sets an explosion on the tile
	void removeExplosion();	//removes the explosion on the tile
	Sprite& getSprite() { return sprite; }
};

