#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<math.h>
#include <iostream>
#include "mapSpace.h"
#include "Player.h"
#include "Wall.h"
#include "Bomb.h"
#include<Windows.h>
#include<fstream>

using namespace sf;

//Forward Declarations//
class Player;
class Wall;
class Bomb;
class mapSpace;
class Enemy;

class Game
{
private:
	//variables//
	RenderWindow* window;
	Event event;

	Clock clock;

	//Textures//
	Texture breakable_t, wall_t, explosion_t, player_t, bomb_t, enemy_t, bg_t, loadScreen_t;
	//Sprites//
	Sprite bg_s, loadScreen_s;

	RectangleShape animationRect;

	//Sounds//
	SoundBuffer bgSoundBuffer, victoryBuffer, bombPlaceBuffer, bombExplodeBuffer;
	Sound bgSound, victorySound, bombPlaceSound, bombExplodeSound;
	
	//Text//
	Text gameOver, gameStart, gameStartShadow;
	Font font;
	std::string gameOverString;

	//Score and Flags//
	int score;
	bool player1Lost, player2Lost, deathAnimationFinished, loading, loadingAnimationDone;

	//Map//
	mapSpace** map;

	//Vectors for bombs and enemies//
	std::vector<Bomb> bombs;
	std::vector<Enemy> enemies;

	//Players//
	Player* player;
	Player* player2;

	//View//
	View view;

	//functions//
	void initVars();
	void initMap();
	void initWindow();
	
public:
	///Constructors and Destructors///
	Game();
	virtual ~Game();

	//Functions//
	void explode(Bomb bomb);

	//Update Functions//
	void updateEnemies();
	void updateBombs();
	void updateExplosions();
	void updatePlayer();
	void animateLoading();
	void update();
	void pollEvents();

	//Render Functions//
	void renderWalls();
	void renderPlayer();
	void renderExplosion();
	void renderBombs();
	void renderEnemies();
	void renderGameOver();
	void animateDeath(int playerNo);
	void render();

	//get methods//
	RenderWindow* getWindow() { return this->window; }
	Event& getEvent() { return event; }

	friend class Player;
};

