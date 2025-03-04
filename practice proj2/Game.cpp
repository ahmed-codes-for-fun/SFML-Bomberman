#include "Game.h"

void Game::initVars()
{
	//booleans//
	player1Lost = false;
	player2Lost = false;
	deathAnimationFinished = false;
	loading = true;

	//loading textures//
	player_t.loadFromFile("player.png");
	explosion_t.loadFromFile("explosion.png");
	enemy_t.loadFromFile("enemy.png");
	bg_t.loadFromFile("bg.png");
	breakable_t.loadFromFile("breakable.png");
	wall_t.loadFromFile("wall.png");
	bomb_t.loadFromFile("bomb.png");
	loadScreen_t.loadFromFile("loadingscreen.png");
	

	//loading Sprites//
	bg_s.setTexture(bg_t);
	bg_s.setColor(Color(255,255,255,125));
	loadScreen_s.setTexture(loadScreen_t);
	loadScreen_s.setScale(Vector2f(1.171f, 1.368f));
	
	//Text Stuff//
	font.loadFromFile("Atop.ttf");

	gameOver.setFont(font);
	gameOver.setFillColor(Color::Red);
	gameOver.setCharacterSize(60);
	gameOver.setPosition(Vector2f(500, 438));
	gameOver.setOutlineColor(Color(253, 253, 150, 255));
	gameOver.setOutlineThickness(10);

	gameStart.setFont(font);
	gameStart.setFillColor(Color::Cyan);
	gameStart.setCharacterSize(80);
	gameStart.setPosition(Vector2f(220, 738));
	gameStart.setOutlineColor(Color::Magenta);
	gameStart.setOutlineThickness(10);
	gameStart.setString("Press Any Key to Begin...");

	gameStartShadow.setFont(font);
	gameStartShadow.setFillColor(Color(50,50,50,200));
	gameStartShadow.setCharacterSize(80);
	gameStartShadow.setPosition(Vector2f(226, 744));
	gameStartShadow.setOutlineColor(Color(50, 50, 50, 200));
	gameStartShadow.setOutlineThickness(10);
	gameStartShadow.setString("Press Any Key to Begin...");

	//Animation Rectangle for initial load//
	animationRect.setSize(Vector2f(1500, 1000));
	animationRect.setFillColor(Color::Black);
	
	//Score//
	score = 0;

	//Loading Players//
	player = new Player();
	player->setTexture(player_t);
	player2 = new Player(2);
	player2->setTexture(player_t);

	//allotting memory to the map//
	map = new mapSpace*[20];
	for (int i = 0; i < 20; i++)
		map[i] = new mapSpace[30];

	//loading enemies//
	enemies.push_back(Enemy('U'));
	enemies.at(0).y = 8;
	enemies.at(0).setTexture(enemy_t);
	//enemy#2//
	enemies.push_back(Enemy('R'));
	enemies.at(1).x = 8;
	enemies.at(1).setTexture(enemy_t);
	
	//Creating View//
	view.setSize(Vector2f(1500, 1000));
	view.setCenter(Vector2f(750, 500));		//to make it span across the entire screen
	//view.setViewport(FloatRect(420.f/1920/2, 80.f/1080/2, 1500.f/1920.f, 1000.f/1080.f));

	//Loading Sounds//
	bgSoundBuffer.loadFromFile("background.mp3");
	victoryBuffer.loadFromFile("victory.mp3");
	if (!bombExplodeBuffer.loadFromFile("Punch Bomb.wav"))
		std::cout << "not loaded\n";
	if(!bombPlaceBuffer.loadFromFile("Item Get.wav"))
		std::cout << "not loaded\n";
	bgSound.setBuffer(bgSoundBuffer);
	victorySound.setBuffer(victoryBuffer);
	bombExplodeSound.setBuffer(bombExplodeBuffer);
	bombPlaceSound.setBuffer(bombPlaceBuffer);
	bgSound.play();
	bgSound.setLoop(true);
}

void Game::initMap()		//Loading level from text file
{
	char arr[20][30];
	std::ifstream fin("example.txt");
	if (!fin)
		std::cout << "not open";
	char ch;
	//Reads file character by character and copies into the array//
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			fin.get(ch);
			if (ch != '\n')		//ignore new line character
				arr[i][j] = ch;
			else
			{
				fin.get(ch);
				if (ch != '\n')
					arr[i][j] = ch;
			}
		}
	}
	fin.close();

	//Populates the map according to the 2d array//
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			std::cout << arr[i][j];
			switch (arr[i][j]) {
			case '|':
				map[i][j].wall = true; 
				map[i][j].setWall(i, j, false, breakable_t, wall_t);
				break;
			case '+':
				map[i][j].breakabaleWall = true;
				map[i][j].setWall(i, j, true, breakable_t, wall_t); break;
			}
		}
		std::cout << std::endl;

		//Sets initial positions of players//
		map[1][1].player1 = true;
		map[18][28].player2 = true;
	}
}

void Game::initWindow()				//initialising window
{
	window = new RenderWindow(VideoMode(1920, 1080), "Flappy 804", Style::Fullscreen);
	window->setFramerateLimit(10);		//Kept the framerate low because of tile movement (made the enemies go crazy fast)
}

Game::Game()		//constructor
{
	initVars();
	initMap();
	initWindow();	
}

Game::~Game()		//destructor
{
	delete window;
}

void Game::explode(Bomb bomb)		//explodes the bombs 
{
	//Activates explosions in all directions till it reaches a wall//
	//Right till 4 spaces//
	for (int i = bomb.x + 1; i < bomb.x + 4; i++)
	{
		if (map[bomb.y][i].wall)		//checking if a wall reaches
		{
			break;
		}
		else
		{
			map[bomb.y][i].explosion = true;
			map[bomb.y][i].setExplosion(bomb.y,i, clock, explosion_t, player, player2, *this);
		}
	}
	//Scanning the left side//
	for (int i = bomb.x; i > bomb.x - 4; i--)
	{
		if (map[bomb.y][i].wall)
		{
			break;
		}
		else
		{
			map[bomb.y][i].explosion = true;
			map[bomb.y][i].setExplosion(bomb.y,i, clock, explosion_t, player, player2, *this);
		}
	}
	//Scanning upwards//
	for (int i = bomb.y - 1; i > bomb.y - 4; i--)
	{
		if (map[i][bomb.x].wall)
		{
			break;
		}
		else
		{
			map[i][bomb.x].explosion = true;
			map[i][bomb.x].setExplosion(i,bomb.x,clock, explosion_t, player, player2, *this);
		}
	}
	//Scanning downwards//
	for (int i = bomb.y; i < bomb.y + 4; i++)
	{
		if (map[i][bomb.x].wall)
		{
			break;
		}
		else
		{
			map[i][bomb.x].explosion = true;
			map[i][bomb.x].setExplosion(i,bomb.x, clock, explosion_t, player,player2,*this);
		}
	}
}

//void Game::updateEnemies()		//now im gonna try for the enemy to follow the player
//{
//	for (int i = 0; i < enemies.size(); i++)
//	{
//		if (player->y > enemies.at(i).y)
//		{
//
//		}
//		else if (player->x > enemies.at(i).x)
//		{
//			if (!map[enemies.at(i).y][enemies.at(i).x + 1].wall && !map[enemies.at(i).y][enemies.at(i).x + 1].breakabaleWall)
//			{
//				enemies.at(i).move('D', map);
//				if (map[enemies.at(i).y][enemies.at(i).x].player1)
//				{
//					std::cout << "Player 1 You lose \n";
//					player->lose(*this);
//					bgSound.stop();
//					victorySound.play();
//				}
//				//Collision detection with player 2//
//				if (map[enemies.at(i).y][enemies.at(i).x].player2)
//				{
//
//					std::cout << "Player 2 You lose \n";
//					player2->lose(*this);
//					bgSound.stop();
//					victorySound.play();
//				}
//				//Collision detection with explosions//
//				if (map[enemies.at(i).y][enemies.at(i).x].explosion)
//				{
//					enemies.erase(enemies.begin() + i);		//removes the enemy from the vector
//					score += 10;	//adds score
//					std::cout << score << std::endl;
//				}
//			}
//
//		}
//		else if (player->x < enemies.at(i).x)
//		{
//			if (!map[enemies.at(i).y][enemies.at(i).x - 1].wall && !map[enemies.at(i).y][enemies.at(i).x - 1].breakabaleWall)		//scanning for walls
//			{
//				enemies.at(i).move('A', map);
//				if (map[enemies.at(i).y][enemies.at(i).x].player1)
//				{
//					std::cout << "Player 1 You lose \n";
//					player->lose(*this);
//					bgSound.stop();
//					victorySound.play();
//
//				}
//				if (map[enemies.at(i).y][enemies.at(i).x].player2)
//				{
//					std::cout << "Player 2 You lose \n";
//					player2->lose(*this);
//					bgSound.stop();
//					victorySound.play();
//
//				}
//				if (map[enemies.at(i).y][enemies.at(i).x].explosion)
//				{
//					//enemies.at(i).removeEnemy();
//					enemies.erase(enemies.begin() + i);		//erase the enemy that collides with explosion
//					score += 10;
//					std::cout << score << std::endl;
//				}
//			}
//		}
//	}
//}
void Game::updateEnemies()			//updates enemy positions
{
	//Checks for all enemies//

	//This code is a little complicated as it tries to keep the enemies going in one direction till they hit the wall
	for (int i=0;i<enemies.size();i++)
	{
		if (enemies.at(i).dir == 1)		//dir will stay 1 till it is going downwards when it reaches the wall it will become 2 going back up
		{
			if (!map[enemies.at(i).y+1][enemies.at(i).x].wall && !map[enemies.at(i).y+1][enemies.at(i).x].breakabaleWall)		//checking if there is a wall
			{
				enemies.at(i).move('S', map);			//calls move function
				//Checking player 1 is colliding with the enemies//
				if (map[enemies.at(i).y][enemies.at(i).x].player1)
				{
					std::cout << "Player 1 You lose \n";
					player->lose(*this);
					bgSound.stop();
					victorySound.play();
				}
				//Collision detection with player 2//
				if (map[enemies.at(i).y][enemies.at(i).x].player2)
				{

					std::cout << "Player 2 You lose \n";
					player2->lose(*this);
					bgSound.stop();
					victorySound.play();
				}
				//Collision detection with explosions//
				if (map[enemies.at(i).y][enemies.at(i).x].explosion)
				{
					enemies.erase(enemies.begin() + i);		//removes the enemy from the vector
					score += 10;	//adds score
					std::cout << score << std::endl;
				}
			}
			else
			{
				enemies.at(i).dir = 2;		//sets direction upwards
			}
		}
		else if (enemies.at(i).dir == 2)		//moves upwards
		{
			if (!map[enemies.at(i).y-1][enemies.at(i).x].wall && !map[enemies.at(i).y-1][enemies.at(i).x].breakabaleWall)		//checks for walls
			{
				enemies.at(i).move('W', map);
				if (map[enemies.at(i).y][enemies.at(i).x].player1)	//collision detection with player 1
				{
					std::cout << "Player 1 You lose \n";
					player->lose(*this);
					bgSound.stop();
					victorySound.play();

				}
				if (map[enemies.at(i).y][enemies.at(i).x].player2)	//collision detection with player 2
				{
					std::cout << "Player 2 You lose \n";
					player2->lose(*this);
					bgSound.stop();
					victorySound.play();
				}
				if (map[enemies.at(i).y][enemies.at(i).x].explosion) //collision detection with explosions
				{
					//enemies.at(i).removeEnemy();
					enemies.erase(enemies.begin() + i);
					score += 10;
					std::cout << score << std::endl;
				}
			}
			else
				enemies.at(i).dir = 1;	//sets direction back downwards
		}
		else if (enemies.at(i).dir == 3)		//moves towards the right
		{
			if (!map[enemies.at(i).y][enemies.at(i).x + 1].wall && !map[enemies.at(i).y][enemies.at(i).x+1].breakabaleWall)		//scanning for walls
			{
				enemies.at(i).move('D', map);
				if (map[enemies.at(i).y][enemies.at(i).x].player1)
				{
					std::cout << "Player 1 You lose \n";
					player->lose(*this);
					bgSound.stop();
					victorySound.play();

				}
				if (map[enemies.at(i).y][enemies.at(i).x].player2)
				{
					std::cout << "Player 2 You lose \n";
					player2->lose(*this);
					bgSound.stop();
					victorySound.play();


				}
				if (map[enemies.at(i).y][enemies.at(i).x].explosion)
				{
					//enemies.at(i).removeEnemy();
					enemies.erase(enemies.begin() + i);
					score += 10;
					std::cout << score << std::endl;
				}
			}
			else
				enemies.at(i).dir = 4;
		}
		else if (enemies.at(i).dir == 4)		//moves left
		{
			if (!map[enemies.at(i).y][enemies.at(i).x - 1].wall && !map[enemies.at(i).y][enemies.at(i).x - 1].breakabaleWall)		//scanning for walls
			{
				enemies.at(i).move('A', map);
				if (map[enemies.at(i).y][enemies.at(i).x].player1)
				{
					std::cout << "Player 1 You lose \n";
					player->lose(*this);
					bgSound.stop();
					victorySound.play();

				}
				if (map[enemies.at(i).y][enemies.at(i).x].player2)
				{
					std::cout << "Player 2 You lose \n";
					player2->lose(*this);
					bgSound.stop();
					victorySound.play();

				}
				if (map[enemies.at(i).y][enemies.at(i).x].explosion)
				{
					//enemies.at(i).removeEnemy();
					enemies.erase(enemies.begin() + i);		//erase the enemy that collides with explosion
					score += 10;
					std::cout << score << std::endl;
				}
			}
			else
				enemies.at(i).dir = 3;		//changes direction
		}
	}
	
	
	
}

//Checks if the bombs have been planted for 3 or more seconds, explodes them if yes and then removes them from the vector//
void Game::updateBombs()			 
{
	for(int i =0;i<bombs.size();i++)
	{
		if (bombs.at(i).update(clock))
		{
			explode(bombs.at(i));			//explodes the bomb
			if (bombs.at(i).playerNo == 1)
				player->no_of_bombs--;		//tracking number of bombs for player 1
			else
				player2->no_of_bombs--;		//tracking player 2's bombs
			bombs.erase(bombs.begin());
			bombExplodeSound.play();
		}
	}
	
}
//Checking collisions of players with explosions and also checking if a second has passed since explosion and removes if yes//
void Game::updateExplosions()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (map[i][j].explosion)
			{
				if (map[i][j].player1)
				{
					player->lose(*this);	//displays player 1 loses
					bgSound.stop();
					victorySound.play();

				}
				else if (map[i][j].player2) {
					player2->lose(*this);		//displayes player 2 loses
					bgSound.stop();
					victorySound.play();

				}
				if (map[i][j].explodeTime + 1 <= clock.getElapsedTime().asSeconds())	//checks if a second has passed
				{
					map[i][j].removeExplosion();		//removes the explosion
				}
			}
		}
	}
}

//function not being used as of yet//
void Game::updatePlayer()
{
	window->setFramerateLimit(60);
	if (player->sprite.getPosition().x < player->x * 50)
	{
		
		player->sprite.move(Vector2f(10, 0));
	}
	else if (player->sprite.getPosition().x > player->x * 50)
	{

		player->sprite.move(Vector2f(-10, 0));
	}
	else if(player->sprite.getPosition().y < player->y * 50)
		player->sprite.move(Vector2f(0, 10));
	else if (player->sprite.getPosition().y > player->y * 50)
		player->sprite.move(Vector2f(0, -10));
	//window->setFramerateLimit(10);
}

//animates the loading screen//
void Game::animateLoading()
{
	window->setFramerateLimit(60);
	//loadScreen_s.setColor(Color(0, 0, 0, 0));
	if(!loading)
		animationRect.move(Vector2f(0, -20));		//moves the black rectangle up revealing the playing field
	if(animationRect.getPosition().y<-1800)
	{
		loadingAnimationDone = true;
		window->setFramerateLimit(10);
	}
}

void Game::update()		//updates the state of all objects
{
	pollEvents();
	//updatePlayer();
	updateEnemies();
	updateBombs();
	updateExplosions();
	if (player1Lost)
		animateDeath(1);		//animates the death of player1
	else if (player2Lost)
		animateDeath(2);		//animates player 2
	if (!loadingAnimationDone)
		animateLoading();		//if loading animation has not been completed, it animtes the screen
	
}

//poll events function to constantly check for events like keys pressed, mouse used or any other such event//
void Game::pollEvents()
{
	while (window->pollEvent(event))
	{
		//checking kind of event//
		switch (event.type)
		{
		case Event::Closed:
			window->close(); break;
		case Event:: KeyPressed:
			loading = false;
			//checking which key was pressed//
			switch (event.key.code)
			{
			case Keyboard::Escape:		//Closes the window
				window->close(); break;
			//calls functions for movement up down left and right//
			case Keyboard::D:
				player->move('D', map);
				break;
			case Keyboard::S:
				player->move('S', map); break;
			case Keyboard::A:
				player->move('A', map); break;
			case Keyboard::W:
				player->move('W', map); break;
			//Player 2's events//
			case Keyboard::Right:
				player2->move('D', map); break;
			case Keyboard::Down:
				player2->move('S', map); break;
			case Keyboard::Left:
				player2->move('A', map); break;
			case Keyboard::Up:
				player2->move('W', map); break;

			//Plants Bombs//
			case Keyboard::F:
				if(player->no_of_bombs <=10)
				{
					bombs.push_back(Bomb(player->y, player->x, 1, clock, bomb_t));		//adds a bomb
					player->no_of_bombs++;		//increases the number of bombs planted by each player
					map[player->y][player->x].bomb = true;
					bombPlaceSound.play();
				}
				break;
			//Plants bombs for player 2//
			case Keyboard::RControl:
				std::cout << "H pressed and number of bombs for player 2 are " << player2->no_of_bombs;
				if(player2->no_of_bombs <= 10)
				{
					std::cout << "player 2 plansts bomb at " << player2->x << " " << player2->y << std::endl;
					bombs.push_back(Bomb(player2->y, player2->x, 2, clock, bomb_t));
					player2->no_of_bombs++;
					map[player2->y][player2->x].bomb = true;
					bombPlaceSound.play();
				}
				break;
			}
		default:
			break;
		}
	}
}

//Coming over to the rendering part//

//Renders Walls on to the screen parsing through the map//
void Game::renderWalls()
{
	for (int i = 0; i < 20;i++) {
		for (int j = 0; j < 30; j++)
		{
			if(map[i][j].wall || map[i][j].breakabaleWall)
			{
				window->draw(map[i][j].sprite);
			}
		}
	}
}

//Renders Players//
void Game::renderPlayer()
{
	window->draw(player->sprite);
	window->draw(player2->sprite);
}

//Renders Explosions//
void Game::renderExplosion()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (map[i][j].explosion)
			{
				window->draw(map[i][j].explosion_s);
			}
		}
	}
}

//Renders bombs//
void Game::renderBombs()
{
	for (int i=0;i<bombs.size();i++)
	{
		window->draw(bombs.at(i).sprite);
	}
}

//Renders Enemies//
void Game::renderEnemies()
{
	for (int i=0;i<enemies.size();i++)
	{
		window->draw(enemies.at(i).sprite);
	}
}

//Renders the Game Over Text//
void Game::renderGameOver()
{
	window->draw(gameOver);
}

//Animates the death of the players//
void Game::animateDeath(int playerNo)
{
	window->setFramerateLimit(60);		//sets the frame rate higher for the animation
	float velocity = 20;		//sets a speed in opposite direction of gravity and then lets the gravity pull it down
	float gravity = 10;
	if (playerNo == 1)
	{
		for (int i = 0; i < 50; i++) 
		{
			player->sprite.move(Vector2f(0, -velocity));
			render();
			velocity -= gravity*0.11;		//0.11 is the multiplier to make the animation smooth
		}
		
	}
	else if (playerNo==2)
	{
		for (int i = 0; i < 50; i++)
		{
			player2->sprite.move(Vector2f(0, -velocity));
			render();
			velocity -= gravity * 0.11;
		}

	}
	deathAnimationFinished = true;
}

//Renders all the objects//
void Game::render()
{
	this->window->clear();
	window->setView(view);
	//draw stuff//
	window->draw(bg_s);
	renderWalls();
	renderBombs();
	renderExplosion();
	renderEnemies();
	renderPlayer();			//draw on top layer
	renderGameOver();
	window->draw(animationRect);
	if(loading)								//loading screen stuff
	{
		window->draw(loadScreen_s);
		window->draw(gameStartShadow);
		window->draw(gameStart);
	}
	

	this->window->display();
	if ((player1Lost || player2Lost) && deathAnimationFinished)		//after death animation pause and close
	{
		Sleep(3000);
		window->close();
	}
}