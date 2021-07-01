#include "Map.h"
#include "Game.hpp"
#include <fstream>
#include "ECS.h"
#include "Components.h"
#include "PathfindingComponent.h"
#include <vector>

extern Manager manager;
std::vector<std::unique_ptr<Vector2D>> points;


Map::Map(std::string tID, int ms, int ts) : textureID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Map::~Map() 
{
	
}

void Map::loadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			addTile(srcX, srcY, x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
	}
	mapFile.ignore();
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			if (c == '1') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("wall", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			if (c == '2') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("lava", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			if (c == 'v') {
				float xpos = static_cast<float>(x * scaledSize);
				float ypos = static_cast<float>(y * scaledSize);
				Vector2D* vec = new Vector2D(xpos, ypos);
				std::unique_ptr<Vector2D> uPtr{ vec };
				points.emplace_back(std::move(uPtr));
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::addTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, textureID);
	tile.addGroup(Game::groupMap);
}