#pragma once
#include "Game.h"
using namespace sf;

//Defines the bombs planted by the character//
class Bomb
{
public:
	int playerNo;		//whihc player planted the bomb
	int x, y;		//coordinates
	RectangleShape rect;		//note being used
	Sprite sprite;			
	float plantTime;		//Stores the time bomb was planted
	Bomb(int i,int j, int playerNumber, Clock clock, Texture& bomb_t);		//Constructor
	~Bomb();
	bool update(Clock clock);		//Checks if bomb is to be exploded 
	void setTexture(Texture& bomb_t);
};

