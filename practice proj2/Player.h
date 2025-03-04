#pragma once
#include"Game.h"
#include "mapSpace.h"
class mapSpace;
using namespace sf;

class Game;

//Defines the player and its various methods//
class Player
{
public:
	Sprite sprite;
	RectangleShape rect;		//not being used
	int x, y;					//coordinates
	int no_of_player;			//player number 1 or 2
	int no_of_bombs;			//no of bombs planted by particular player

	Player();
	Player(int no_of_players);
	virtual void Map(mapSpace** map);		//adds the location on the map array
	virtual void RemoveMap(mapSpace** map);		//removes it from the array
	virtual void setTexture(Texture& x);		//sets texture
	void move(char dir, mapSpace** map);		//moves checking if there isnt a wall
	virtual void updatePosition();				//updates position as per x,y coordinate of array and translates into pixel coords
	void lose(Game& obj);						//lose condition
};


//Enemy class inherited from player//
class Enemy : public Player {
public:
	int dir;	//1 for down 2 for up 3 for right and 4 for left (complicated logic)
	Enemy();
	Enemy(char dir);
	void Map(mapSpace** map);
	void RemoveMap(mapSpace** map);
	void setTexture(Texture& x);
	void updatePosition();
};