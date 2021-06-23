#include "Game.hpp"
#include "Texturemanager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"


Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,1280, 800 };

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

const char* mapfile = "assets/terrain_ss.png";

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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
	map = new Map();
	
	Map::loadMap("assets/40x25.map",40,25);
	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("assets/Rambo_SpriteSheet.png",true); // player sprite sheet	Rambo_SpriteSheet.png
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
	

}
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
	manager.refresh();
	manager.update();

	camera.x = player.getComponent<TransformComponent>().position.x - 640;
	camera.y = player.getComponent<TransformComponent>().position.y - 400;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

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
	SDL_RenderPresent(renderer);
}
void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}
void Game::addTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
	tile.addGroup(groupMap);
}