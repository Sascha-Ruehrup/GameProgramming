#include "Game.hpp"
#include "Texturemanager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <map>
#include <sstream>
#include <random>
Map* map;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,1280, 800 };

AssetManager* Game::assets = new AssetManager(&manager);
bool Game::isRunning = false;
bool gameStarted = false;
bool firstUpdate = true;
bool drawGameOver = false;
Vector2D* Game::playerPosition = new Vector2D(0.0f,0.0f);
int Game::playerWeapon = 0;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& healthbar(manager.addEntity());
auto& weapon(manager.addEntity());
auto& score(manager.addEntity());
auto& wave(manager.addEntity());
auto& startbutton(manager.addEntity());
auto& gameOver(manager.addEntity());

int health = 100;
int scoreValue = 0;
int waveValue = 1;
int waveZombieCounter = 30;
Vector2D* spawnPoints[] = { new Vector2D(0.0f,700.0f), new Vector2D(1200.0f, 0.0f),new Vector2D(2500.0f,700.0f) };

int timer = 60;
int xMousepos = 0;
int yMousepos = 0;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Inititalised!" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Windiw created!" << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF" << std::endl;
	}
	assets->addTexture("terrain", "assets/terrain_ss.png");
	assets->addTexture("player", "assets/Rambo_SpriteSheet.png");
	assets->addTexture("zombie", "assets/zombie.png");
	assets->addTexture("healthbar", "assets/healthbar.png");
	assets->addTexture("RifleProjectileSideways", "assets/projectilesideways.png");
	assets->addTexture("RifleProjectileUp", "assets/projectileup.png");
	assets->addTexture("rifle", "assets/rifle.png");
	assets->addTexture("startButton", "assets/startbutton.png");
	assets->addTexture("gameOver", "assets/gameover.png");
	assets->addTexture("RocketLauncherProjectileSideways", "assets/rocketProjectileSideways.png");
	assets->addTexture("RocketLauncherProjectileUp", "assets/rocketProjectileUp.png");
	assets->addTexture("RocketLauncherProjectileDown", "assets/rocketProjectileDown.png");
	

	assets->addFont("arial", "assets/SHOWG.ttf", 48);
	assets->addFont("arial", "assets/SHOWG.ttf", 64);
	map = new Map("terrain", 2, 32);
	
	map->loadMap("assets/40x25.map",40,25);

	player.addComponent<TransformComponent>(4, 250, 320);
	player.addComponent<SpriteComponent>("player",true); // player sprite sheet	Rambo_SpriteSheet.png
	player.addComponent<WeaponComponent>(Game::rocketLauncher);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<HealthManagementComponent>(100);
	player.addGroup(groupPlayers);

	healthbar.addComponent<TransformComponent>(0, 0, 10, 100, 4);
	healthbar.addComponent<SpriteComponent>("healthbar");
	healthbar.addGroup(groupUI);
	weapon.addComponent<TransformComponent>(0, 600, 32, 32, 8);
	weapon.addComponent<SpriteComponent>("rifle");
	weapon.addGroup(groupUI);
	SDL_Color black = { 0,0,0,255 };
	score.addComponent<UILabel>(500, 60, "Score: ", "arial", black);
	score.addGroup(groupUI);
	wave.addComponent<UILabel>(500, 10, "Wave: ", "arial", black);
	wave.addGroup(groupUI);
	startbutton.addComponent<TransformComponent>(376, 384, 32, 128, 4);
	startbutton.addComponent<SpriteComponent>("startButton");
	gameOver.addComponent<TransformComponent>(376, 220, 64, 256, 2);
	gameOver.addComponent<SpriteComponent>("gameOver");

}


auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& UI(manager.getGroup(Game::groupUI));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));
void Game::handleEvents()
{
	
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			if (!gameStarted) {
				xMousepos = Game::event.motion.x;
				yMousepos = Game::event.motion.y;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (!gameStarted) {
				if (xMousepos > 376 && xMousepos < 888 && yMousepos > 384 && yMousepos < 512) {
					std::cout << "Mouse clicked" << std::endl;
					newGame();
				}
			}
			break;
		default:
			break;
	}
}

void Game::newGame() {
	for (auto& e : enemies) {
		e->destroy();
	}
	player.getComponent<TransformComponent>().position.x = 250;
	player.getComponent<TransformComponent>().position.y = 320;

	int playerHealth = 100;
	player.getComponent<HealthManagementComponent>().maximumHealth = playerHealth;
	health = playerHealth;
	healthbar.getComponent<SpriteComponent>().srcRect.w = playerHealth;
	healthbar.getComponent<TransformComponent>().width = playerHealth;

	scoreValue = 0;
	waveValue = 1;
	waveZombieCounter = 30;

	timer = 180;

	drawGameOver = false;
	gameStarted = true;
}


void Game::update() {
	if (!gameStarted) {
		if (firstUpdate) {
			manager.refresh();
			manager.update();
			firstUpdate = false;
		}
		
	}
	else
	{
		timer--;
		if (waveZombieCounter <= 0) {
			waveValue++;
			waveZombieCounter = 30 + waveValue * 5;
		}
		if (timer == 0) {
			spawnZombieAtRandomPosition();
			if (180 - waveValue * 5 >= 60) {
				timer = 180 - waveValue * 5;
			}
			else
			{
				timer = 60;
			}

		}
		SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
		Vector2D playerPos = player.getComponent<TransformComponent>().position;

		Game::playerPosition = &playerPos;
		Game::playerWeapon = player.getComponent<WeaponComponent>().weapon;

		std::map<std::size_t, Vector2D>enemiesPositions;
		for (auto& e : enemies) {
			enemiesPositions.insert(std::make_pair(e->ID, e->getComponent<TransformComponent>().position));
		}
		manager.refresh();
		manager.update();

		for (auto& c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol)) {
				if (c->getComponent<ColliderComponent>().tag == "wall") {
					player.getComponent<TransformComponent>().position = playerPos;
				}
				else if (c->getComponent<ColliderComponent>().tag == "lava") {
					health -= 1;
					updateHealthbar(1);
					if (health >= 1) {
						player.getComponent<HealthManagementComponent>().maximumHealth -= 1;
						std::cout << "I AM BURNING!" << std::endl;
					}
					else if (health <= 0)
					{
						drawGameOver = true;
						std::cout << " Tot" << std::endl;
						gameStarted = false;
						//TODO
						//endGame();
					}

				}
			}
		}
		for (auto& e : enemies) {
			if (Collision::AABB(e->getComponent<ColliderComponent>().collider, playerCol))
			{
				health -= 1;
				updateHealthbar(1);
				if (health >= 1) {
					player.getComponent<HealthManagementComponent>().maximumHealth -= 1;
					std::cout << "I AM BURNING!" << std::endl;
				}
				else if (health <= 0)
				{
					drawGameOver = true;
					std::cout << " Tot" << std::endl;
					gameStarted = false;
					//TODO
					//endGame();
				}
			}
		}

		for (auto& p : projectiles)
		{
			for (auto& e : enemies) {
				if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
				{
					e->getComponent<HealthManagementComponent>().maximumHealth -= 1;
					if (e->getComponent<HealthManagementComponent>().maximumHealth <= 0) {
						e->destroy();
						waveZombieCounter--;
						scoreValue++;
					}
					p->destroy();

				}
			}

		}
		std::stringstream sv, wv;
		sv << "SCORE: " << scoreValue;
		score.getComponent<UILabel>().setLabelText(sv.str(), "arial");
		wv << "WAVE: " << waveValue;
		wave.getComponent<UILabel>().setLabelText(wv.str(), "arial");
		camera.x = player.getComponent<TransformComponent>().position.x - 640;
		camera.y = player.getComponent<TransformComponent>().position.y - 400;

		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.y < 0) {
			camera.y = 0;
		}
		if (camera.x > camera.w) {
			camera.x = camera.w;
		}
		if (camera.y > camera.h) {
			camera.y = camera.h;

		}
		healthbar.getComponent<TransformComponent>().position.x = camera.x;
		healthbar.getComponent<TransformComponent>().position.y = camera.y;
		weapon.getComponent<TransformComponent>().position.x = camera.x;
		weapon.getComponent<TransformComponent>().position.y = camera.y + 600;
		startbutton.getComponent<TransformComponent>().position.x = camera.x + 376;
		startbutton.getComponent<TransformComponent>().position.y = camera.y + 384;
		gameOver.getComponent<TransformComponent>().position.x = camera.x + 376;
		gameOver.getComponent<TransformComponent>().position.y = camera.y + 220;
	}
}
void Game::updateHealthbar(int damage) {
	healthbar.getComponent<SpriteComponent>().srcRect.w -= damage;
	healthbar.getComponent<TransformComponent>().width -= damage;
}

void Game::render()
{
	SDL_RenderClear(renderer);
		for (auto& t : tiles)
		{
			t->draw();
		}
		for (auto& p : players)
		{
			p->draw();
		}
		for (auto& e : enemies)
		{
			e->draw();
		}
		for (auto& p : projectiles)
		{
			p->draw();
		}
		for (auto& ui : UI)
		{
			ui->draw();
		}
		if (!gameStarted) {
			startbutton.draw();
		}
		if (drawGameOver) {
			gameOver.draw();
		}
	
	
	
	SDL_RenderPresent(renderer);
}
void Game::spawnZombie(int xpos, int ypos)
{
	auto& zombie(manager.addEntity());
	zombie.addComponent<TransformComponent>(4, xpos, ypos);
	zombie.addComponent<SpriteComponent>("zombie", false);
	zombie.addComponent<ColliderComponent>("zombie");
	zombie.addComponent<HealthManagementComponent>(2);
	zombie.addComponent<PathfindingComponent>(map->getPoints(), map->getPaths());
	zombie.addGroup(groupEnemies);
}
void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}
int Game::createRandomNumber(int lowestValue, int highestValue) {
	int randomNumber = rand() % ((highestValue - lowestValue) + 1) + lowestValue;
	return randomNumber;
}
void Game::spawnZombieAtRandomPosition() {
	int index = Game::createRandomNumber(0, 2);
	Vector2D* spawnPos = spawnPoints[index];
	Game::spawnZombie(spawnPos->x, spawnPos->y);
}


