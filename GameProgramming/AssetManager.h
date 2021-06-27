#pragma once

#include <map>
#include <string>
#include "Texturemanager.h"
#include "Vector2D.h"
#include"ECS.h"

class AssetManager {
public:
	AssetManager(Manager* man);
	~AssetManager();

	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip);




	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(std::string id);
private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
};