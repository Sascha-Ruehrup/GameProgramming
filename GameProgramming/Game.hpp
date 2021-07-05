#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Map.h"

class AssetManager;
class ColliderComponent;
class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();
	void updateHealthbar(int damage);

	bool running() { return isRunning; };
	
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static Vector2D* playerPosition;

	static AssetManager* assets;
	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupUI
	};
private:
	SDL_Window* window;
	Map* map;
};

#endif // !Game_hpp
