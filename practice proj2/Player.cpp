#include "Player.h"

//Constructor
Player::Player()
{
	no_of_player = 1;
	no_of_bombs = 0;
	x = 1;
	y = 1;
	
	rect.setSize(Vector2f(50, 50));
	rect.setFillColor(Color::Blue);
	updatePosition();
}

//Constructor for player 2//
Player::Player(int no_of_players)
{
	no_of_player = no_of_players;
	if(no_of_players==1)
	{
		x = 1;
		y = 1;
	}
	else if (no_of_players==2)
	{
		x = 28; y = 18;
	}
	no_of_bombs = 0;
	rect.setSize(Vector2f(50, 50));
	rect.setFillColor(Color::Blue);
	updatePosition();
}

//Sets the boolean flags//
void Player::Map(mapSpace** map)
{
	if(no_of_player==1)
	{
		map[y][x].player1 = true;
	}
	else if (no_of_player == 2)
	{
		map[y][x].player2 = true;
	}
}

//Resets boolean flags//
void Player::RemoveMap(mapSpace** map)
{
	if (no_of_player == 1)
	{
		map[y][x].player1 = false;
	}
	else if (no_of_player == 2)
	{
		map[y][x].player2 = false;
	}
}


void Player::setTexture(Texture& x)
{
	sprite.setTexture(x);
	sprite.setScale(Vector2f(0.232, 0.1557));
}

void Enemy::setTexture(Texture& x)
{
	sprite.setTexture(x);
	sprite.setScale(Vector2f(0.148, 0.148));
	updatePosition();
}

//Moves the character in the map while scanning for walls//
void Player::move(char dir, mapSpace** map)
{
	switch (dir)
	{
	case 'D':
		if (x < 28 && !(map[y][x + 1].wall || map[y][x + 1].breakabaleWall))		//scans for walls
		{ 
			this->RemoveMap(map);		//removes from old coordinates
			x++;						//increments x coordinate
			std::cout << x << " incremented \n";
			this->Map(map);				//adds to the new coordinates
			//std::cout << x << " " << y << '\n';
		}
		else
			std::cout << "ye ho rha hai\n";
		break;
	case 'A':		//left
		if (x > 1 && !(map[y][x - 1].wall || map[y][x - 1].breakabaleWall))
		{
			this->RemoveMap(map);
			x--;
			this->Map(map);
			
		}
		//std::cout << x << " " << y << '\n';
		break;
	case 'W':		//up
		if (y > 1 && !(map[y - 1][x].wall || map[y - 1][x].breakabaleWall))
		{
			
			this->RemoveMap(map);
			y--;
			this->Map(map);
			
		}
		//std::cout << x << " " << y << '\n';
		break;
	case 'S':		//down
		if (y < 18 && !(map[y + 1][x].wall || map[y + 1][x].breakabaleWall))
		{
			
			this->RemoveMap(map);
			y++;
			this->Map(map);
			
		}
		//std::cout << x << " " << y << '\n';
		break;
	}
	updatePosition();
}

void Player::updatePosition()
{
	sprite.setPosition(Vector2f(x * 50, y * 50));
}

//Sets game over text and sets flag for loss to true//
void Player::lose(Game& obj)
{
	if (no_of_player == 1)
	{
		obj.gameOverString = "Player 1 loses!!!";
		obj.player1Lost = true;
	}

	else
	{
		obj.gameOverString = "Player 2 loses!!!";
		obj.player2Lost = true;
	}
	obj.gameOver.setString(obj.gameOverString);
	
}


void Enemy::updatePosition()
{
	sprite.setPosition(Vector2f(x * 50, y * 50));

}

Enemy::Enemy()
{
	rect.setFillColor(Color::Cyan);
}

//Defines two kinds of enemies//
Enemy::Enemy(char d)
{
	//Moves up n down//
	if (d == 'U')
	{
		this->dir = 1;
	}
	//Moves left and Right//
	else
	{
		dir = 3;
	}
}

//Overloaded map and remove map functions//
void Enemy::Map(mapSpace** map)
{
	map[y][x].enemy = true;
}

void Enemy::RemoveMap(mapSpace** map)
{
	map[y][x].enemy = false;
}
