#include "Game.hpp"
#include "Texturemanager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

Map* map;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,1280, 800 };

AssetManager* Game::assets = new AssetManager(&manager);
bool Game::isRunning = false;


auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& healthbar(manager.addEntity());
int health = 100;


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
	assets->addTexture("terrain", "assets/terrain_ss.png");
	assets->addTexture("player", "assets/Rambo_SpriteSheet.png");
	assets->addTexture("healthbar", "assets/healthbar.png");
	assets->addTexture("projectileSideways", "assets/projectilesideways.png");
	assets->addTexture("projectileUp", "assets/projectileup.png");
	map = new Map("terrain", 2, 32);
	
	map->loadMap("assets/40x25.map",40,25);
	player.addComponent<TransformComponent>(4, 250, 320);
	player.addComponent<SpriteComponent>("player",true); // player sprite sheet	Rambo_SpriteSheet.png
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
	healthbar.addComponent<TransformComponent>(0, 0, 10, 100, 4);
	healthbar.addComponent<SpriteComponent>("healthbar");
	healthbar.addGroup(groupUI);

	

}
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& UI(manager.getGroup(Game::groupUI));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}
void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	int damage = 0;
	manager.refresh();
	manager.update();

	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			if (c->getComponent<ColliderComponent>().tag == "wall") {
				player.getComponent<TransformComponent>().position = playerPos;
			}
			else if (c->getComponent<ColliderComponent>().tag == "lava") {
				damage += 1;
				std::cout << "I AM BURNING!" << std::endl;
			}
		}
	}
	/*for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			damage += 10;
			p->destroy();

		}
	}*/
	updateHealthbar(damage);
	health -= damage;
	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x) - 640;
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y) - 400;
	
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
	healthbar.getComponent<TransformComponent>().position.x = static_cast<float>(camera.x);
	healthbar.getComponent<TransformComponent>().position.y = static_cast<float>(camera.y);
	

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
	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& ui : UI)
	{
		ui->draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}
