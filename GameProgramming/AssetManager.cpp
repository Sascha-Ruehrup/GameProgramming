#include "AssetManager.h"
#include "Components.h"
AssetManager::AssetManager(Manager* man) : manager(man)
{

}
AssetManager::~AssetManager()
{

}
void AssetManager::createProjectile(int weapon,Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, flip);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addComponent<WeaponComponent>(weapon);
	projectile.addGroup(Game::groupProjectiles);
}


void AssetManager::addTexture(std::string id, const char* path) {
	textures.emplace(id, TextureManager::LoadTexture(path));
}
SDL_Texture* AssetManager::getTexture(std::string id)
{
	return textures[id];
}
void AssetManager::addFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* AssetManager::getFont(std::string id)
{
	return fonts[id];
}

Mix_Music* AssetManager::getMusic(std::string filename) {

	std::string fullPath = "assets/";
	fullPath.append(filename);
	if (mMusic[fullPath] == nullptr) {
		mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());
		if (mMusic[fullPath] == NULL) {
			printf("Music loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
		}
	}
	return mMusic[fullPath];
}

Mix_Chunk* AssetManager::getSFX(std::string filename) {
	std::string fullPath = "assets/";
	fullPath.append(filename);
	
	if (mSFX[fullPath] == nullptr) {

		mSFX[fullPath] = Mix_LoadWAV(fullPath.c_str());
		if (mSFX[fullPath] == NULL) {
			printf("SFX loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
		}
	}
	return mSFX[fullPath];
}