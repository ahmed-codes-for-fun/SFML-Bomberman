#pragma once
#include "Game.h"
using namespace sf;

//Class not being used//
class Wall
{
public:
	Sprite sprite;
	RectangleShape rect;
	int x, y;
	bool breakable;
	void set(int i, int j, bool a);
	void updatePosition();
	Sprite& getSprite() { return sprite; }
	Wall();
};

