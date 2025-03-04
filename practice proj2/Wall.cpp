#include "Wall.h"


//Class not being used//
void Wall::set(int i, int j, bool a)
{
	x = j; y = i; breakable = a;
	if (!breakable)
		rect.setFillColor(Color::Red);
	else
		rect.setFillColor(Color::Yellow);
	updatePosition();
}

void Wall::updatePosition()
{
	rect.setPosition(x * 50, y * 50);
}

Wall::Wall()
{
	rect.setSize(Vector2f(50, 50));
}
