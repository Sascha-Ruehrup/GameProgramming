#include "Game.hpp"
#include "Texturemanager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include <map>
#include <sstream>
#include <random>


Manager manager;

Map* map;
AssetManager* Game::assets = new AssetManager(&manager);
AudioManager* Game::mAudioMgr = new AudioManager();
SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;
SDL_Rect Game::camera = { 0,0,1280, 800 };

bool Game::isRunning = false;
bool gameStarted = false;
bool firstUpdate = true;
bool drawGameOver = false;

Vector2D* Game::playerPosition = new Vector2D(250.0f,320.0f);
std::vector<Vector2D*> spawnPoints = { new Vector2D(-150.0f,560.0f), new Vector2D(1080.0f, -150.0f),
										new Vector2D(2560.0f,560.0f), new Vector2D(1080.0f, 1610.0f) };

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& healthbar(manager.addEntity());
auto& weapon(manager.addEntity());
auto& score(manager.addEntity());
auto& wave(manager.addEntity());
auto& startbutton(manager.addEntity());
auto& gameOver(manager.addEntity());
auto& rocketLauncherAmmunitionDisplay(manager.addEntity());

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& UI(manager.getGroup(Game::groupUI));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& explosions(manager.getGroup(Game::groupExplosions));
auto& items(manager.getGroup(Game::groupItems));
auto& bloods(manager.getGroup(Game::groupBlood));

int Game::playerWeapon = Game::rifle;
int Game::rocketAmmunition = 0;
int Game::volume = 32;
int scoreValue = 0;
int waveValue = 1;
int waveZombieCounter = 30;
int timer = 60;
int explosionTimer = 10;
int xMousepos = 0;
int yMousepos = 0;

Game::Game() {}

Game::~Game()
{
	AudioManager::Release();
	mAudioMgr = NULL;
}

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
	assets->addTexture("rocketLauncher", "assets/rocketLauncher.png");
	assets->addTexture("startButton", "assets/startbutton.png");
	assets->addTexture("gameOver", "assets/gameover.png");
	assets->addTexture("RocketLauncherProjectileSideways", "assets/rocketProjectileSideways.png");
	assets->addTexture("RocketLauncherProjectileUp", "assets/rocketProjectileUp.png");
	assets->addTexture("RocketLauncherProjectileDown", "assets/rocketProjectileDown.png");
	assets->addTexture("explosion", "assets/explosion.png");
	assets->addTexture("itemHealth", "assets/healthItem.png");
	assets->addTexture("itemRockets", "assets/rocketItem.png");
	assets->addTexture("blood", "assets/blood.png");

	assets->addFont("showg48", "assets/SHOWG.ttf", 48);

	map = new Map("terrain", 2, 32);
	map->loadMap("assets/40x25.map",40,25);

	player.addComponent<TransformComponent>(4, 250, 320);
	player.addComponent<SpriteComponent>("player",true);
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

	// create UI elements
	SDL_Color black = { 0,0,0,255 };
	score.addComponent<UILabel>("score", 500, 60, "Score: ", "showg48", black);
	score.addGroup(groupUI);
	wave.addComponent<UILabel>("wave", 500, 10, "Wave: ", "showg48", black);
	wave.addGroup(groupUI);
	rocketLauncherAmmunitionDisplay.addComponent<UILabel>("rlAmmo",0, 520, "ROCKETS: ", "showg48", black);
	rocketLauncherAmmunitionDisplay.addGroup(groupUI);

	startbutton.addComponent<TransformComponent>(376, 384, 32, 128, 4);
	startbutton.addComponent<SpriteComponent>("startButton");
	gameOver.addComponent<TransformComponent>(376, 220, 64, 256, 2);
	gameOver.addComponent<SpriteComponent>("gameOver");

	// initial Audio
	mAudioMgr->playMusic("Playboi Carti - Magnolia.wav");
	mAudioMgr->pauseMusic();
	// music volume
	Mix_VolumeMusic(2);
	// set volume of all SFX channels
	Mix_Volume(-1, volume);
	// footstep sound
	Mix_Volume(0, volume * 2);
}


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
	for (auto& i : items) {
		i->destroy();
	}
	for (auto& p : projectiles) {
		p->destroy();
	}
	for (auto& b : bloods) {
		b->destroy();
	}
	TransformComponent* transform = &player.getComponent<TransformComponent>();
	transform->position.x = 250;
	transform->position.y = 320;
	transform->velocity.x = 0;
	transform->velocity.y = 0;

	int playerHealth = 100;
	player.getComponent<HealthManagementComponent>().maximumHealth = playerHealth;
	healthbar.getComponent<SpriteComponent>().srcRect.w = playerHealth;
	healthbar.getComponent<TransformComponent>().width = playerHealth;

	playerWeapon = 0;
	rocketAmmunition = 5;

	scoreValue = 0;
	waveValue = 1;
	waveZombieCounter = 30;

	timer = 180;

	Mix_Resume(0);
	mAudioMgr->resumeMusic();

	drawGameOver = false;
	gameStarted = true;
}


void Game::update() {
	switch (Game::playerWeapon) {
	case Game::rifle:
		weapon.getComponent<SpriteComponent>().setTex("rifle");
		break;
	case Game::rocketLauncher:
		weapon.getComponent<SpriteComponent>().setTex("rocketLauncher");
		break;
	default:
		weapon.getComponent<SpriteComponent>().setTex("rifle");
		break;
	}

	if (!gameStarted) {
		if (firstUpdate) {
			manager.refresh();
			manager.update();
			firstUpdate = false;
		}
	}
	else
	{
		explosionTimer--;
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

		manager.refresh();
		manager.update();

		// keep player inside the map
		Vector2D playerPosUpdated = player.getComponent<TransformComponent>().position;
		if ((playerPosUpdated.x <= 0) || (playerPosUpdated.x >= 2440) || (playerPosUpdated.y <= 0) || (playerPosUpdated.y>=1470) ) {
			player.getComponent<TransformComponent>().position = playerPos;
		}

		for (auto& c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol)) {
				if (c->getComponent<ColliderComponent>().tag == "wall") {
					player.getComponent<TransformComponent>().position = playerPos;
				}
				else if (c->getComponent<ColliderComponent>().tag == "lava") {
					updateHealth(1);
				}
			}
		}

		for (auto& i : items) {
			if (Collision::AABB(player.getComponent<ColliderComponent>().collider, i->getComponent<ColliderComponent>().collider))
			{
				if (i->getComponent<ColliderComponent>().tag == "itemHealth") {
					updateHealth(-(i->getComponent<ItemComponent>().health));
				}
				else if (i->getComponent<ColliderComponent>().tag == "itemRockets") {
					rocketAmmunition += i->getComponent<ItemComponent>().rockets;
				}
				i->destroy();
			}
		}

		for (auto& e : enemies) {
			if (Collision::AABB(e->getComponent<ColliderComponent>().collider, playerCol))
			{
				updateHealth(1);
			}
		}

		for (auto& p : projectiles)
		{
			for (auto& e : enemies) {
				if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
				{
					TransformComponent transform = e->getComponent<TransformComponent>();
					if (p->getComponent<WeaponComponent>().weapon == Game::rifle) {

						auto& blood(manager.addEntity());
						blood.addComponent<TransformComponent>(transform.position.x, transform.position.y, 32, 32, 4);
						blood.addComponent<SpriteComponent>("blood");
						blood.getComponent<SpriteComponent>().setAngle(Game::createRandomNumber(0, 360));
						blood.addGroup(groupBlood);
						damageZombie(e,1);

					}
					else if (p->getComponent<WeaponComponent>().weapon == Game::rocketLauncher) {
						int xpos = static_cast<int>(p->getComponent<TransformComponent>().position.x);
						int ypos = static_cast<int>(p->getComponent<TransformComponent>().position.y);

						auto& explosion(manager.addEntity());
						explosion.addComponent<TransformComponent>(xpos - 80, ypos - 80, 32, 32, 6);
						explosion.addComponent<SpriteComponent>("explosion");
						explosion.addComponent<ColliderComponent>("explosion", xpos, ypos, 96, true);
						explosion.addGroup(groupExplosions);

						Game::mAudioMgr->playSFX("explosion.wav", 0, -1);
						explosionTimer = 10;
					}
					p->destroy();

				}
			}
		}
		for (auto& ex : explosions) {
			for (auto& e : enemies) {
				if (Collision::CircleRect(ex->getComponent<ColliderComponent>().colliderCircle, e->getComponent<ColliderComponent>().collider)) {
					damageZombie(e, 5);
				}
			}
		}

		// update displayed scores, numbers and counters
		std::stringstream sv, wv, rl;
		sv << "SCORE: " << scoreValue;
		score.getComponent<UILabel>().setLabelText(sv.str(), "showg48");
		wv << "WAVE: " << waveValue;
		wave.getComponent<UILabel>().setLabelText(wv.str(), "showg48");
		rl << "ROCKETS: " << rocketAmmunition;
		rocketLauncherAmmunitionDisplay.getComponent<UILabel>().setLabelText(rl.str(), "showg48");


		// adjust camera position relative to player's position
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

		placeUI(healthbar, 0, 0);
		placeUI(weapon, 0, 600);
		placeUI(startbutton, 376, 384);
		placeUI(gameOver, 376, 220);
	}
}


void Game::placeUI(Entity& eEntity, int xpos, int ypos) {
	Vector2D* position = &eEntity.getComponent<TransformComponent>().position;
	position->x = static_cast<float>(Game::camera.x + xpos);
	position->y = static_cast<float>(Game::camera.y + ypos);
}


void Game::playerGameOver() {

	mAudioMgr->pauseMusic();

	// stop SFX of all channels
	Mix_Pause(-1);
	//Mix_HaltChannel(-1);

	drawGameOver = true;
	gameStarted = false;
}


void Game::updateHealth(int damage) {
	int* health = &player.getComponent<HealthManagementComponent>().maximumHealth;
	*health -= damage;
	if (*health > 100) {
		*health = 100;
	}
	healthbar.getComponent<SpriteComponent>().srcRect.w = *health;
	healthbar.getComponent<TransformComponent>().width = *health;
	if (*health <= 0) {
		playerGameOver();
		return;
	}
}


void Game::damageZombie(Entity*& eEntity, int damage) {
	int *health = &eEntity->getComponent<HealthManagementComponent>().maximumHealth;
	*health -= damage;
	if (*health <= 0) {
		TransformComponent transform = eEntity->getComponent<TransformComponent>();
		Game::dropItem(4, static_cast<int>(transform.position.x), static_cast<int>(transform.position.y));
		waveZombieCounter--;
		scoreValue++;
		eEntity->destroy();
	}
}


void Game::render()
{
	SDL_RenderClear(renderer);
		for (auto& t : tiles)
		{
			t->draw();
		}
		for (auto& b : bloods)
		{	
			b->draw();
		}
		for (auto& i : items)
		{
			i->draw();
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
		for (auto& ex : explosions) {
			if (explosionTimer > 0) {
				ex->draw();
			}
			else {
				ex->destroy();
			}
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
	int index = Game::createRandomNumber(0, spawnPoints.size()-1);
	Vector2D* spawnPos = spawnPoints[index];
	Game::spawnZombie(static_cast<int>(spawnPos->x), static_cast<int>(spawnPos->y));
}


void Game::dropItem(int probability, int xpos, int ypos) {
	int i = Game::createRandomNumber(0, probability);
	if (i == probability) {

		auto& item(manager.addEntity());
		item.addGroup(groupItems);
		item.addComponent<TransformComponent>(1, xpos, ypos);
		int itemNumber = Game::createRandomNumber(0, 1);
		switch(itemNumber){
			case 0:
				//drop rockets
				item.addComponent<SpriteComponent>("itemRockets");
				item.addComponent<ColliderComponent>("itemRockets", xpos, ypos, 32);
				item.addComponent<ItemComponent>(0, 3);
				break;
			case 1:
				//drop health
				item.addComponent<SpriteComponent>("itemHealth");
				item.addComponent<ColliderComponent>("itemHealth", xpos, ypos, 32);
				item.addComponent<ItemComponent>(20, 0);
				break;
			default:
				break;
		}
	}
}

