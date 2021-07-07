#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include "Map.h"
#include "AudioManager.h"
#include <SDL_ttf.h>
#include "ECS.h"


class AssetManager;
class ColliderComponent;
class AudioManager;
class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void newGame();
	void update();
	void render();
	void clean();
	void updateHealth(int damage);
	void spawnZombie(int xpos, int ypos);
	bool running() { return isRunning; };
	static int createRandomNumber(int lowestValue, int highestValue);
	void playerGameOver();
	void killZombie(Entity*& eEntity);
	void spawnZombieAtRandomPosition();
	void dropItem(int probability, int xpos, int ypos);
	static void placeUI(Entity& eEntity, int xpos, int ypos);

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static Vector2D* playerPosition;
	static AssetManager* assets;
	static int playerWeapon;
	static int rocketAmmunition;
	static int volume;
	static AudioManager* mAudioMgr;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupBlood,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupItems,
		groupProjectiles,
		groupExplosions,
		groupUI
	};
	enum weapons : int
	{
		rifle,
		rocketLauncher
	};
private:
	SDL_Window* window;
	Map* map;
};

#endif // !Game_hpp
