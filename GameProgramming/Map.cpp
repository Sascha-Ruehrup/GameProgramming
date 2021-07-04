#include "Map.h"
#include "Game.hpp"
#include <fstream>
#include "ECS.h"
#include "Components.h"
#include "PathfindingComponent.h"
#include <vector>

extern Manager manager;
std::vector<Vector2D*> points;
std::vector<std::vector<int>> paths;


Map::Map(std::string tID, int ms, int ts) : textureID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
	init();
}

Map::~Map() 
{
	
}

void Map::loadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	//char array[2];
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
				points.push_back(vec);
			}
			mapFile.ignore();
			/*for (int q = 0; q < points.size(); q++) {
				mapFile.get(c);
				int i = 1;
				while (c != ',') {
					array[i] = c;
					i--;
					mapFile.get(c);
				}

			}*/
			
		}
	}
	mapFile.close();
}

void Map::init() {
	std::vector<int> vec0{ 1,8 };
	std::vector<int> vec1{ 0,2,4,5,9,12};
	std::vector<int> vec2{ 1,3,4,5,10,13 };
	std::vector<int> vec3{ 1,2,7,14 };
	std::vector<int> vec4{ 1,2,5,9,12 };
	std::vector<int> vec5{ 1,2,4,6,10 };
	std::vector<int> vec6{ 2,5,7,10,13 };
	std::vector<int> vec7{ 3,6,10,13,14 };
	std::vector<int> vec8{ 0,9,11,12 };
	std::vector<int> vec9{ 1,4,8,11,12 };
	std::vector<int> vec10{ 2,5,6,7,13,14 };
	std::vector<int> vec11{ 8,9,12,13,14 };
	std::vector<int> vec12{ 1,4,8,9,11,13,14 };
	std::vector<int> vec13{ 2,6,7,10,11,12,14 };

	paths.push_back(vec0);
	paths.push_back(vec1);
	paths.push_back(vec2);
	paths.push_back(vec3);
	paths.push_back(vec4);
	paths.push_back(vec5);
	paths.push_back(vec6);
	paths.push_back(vec7);
	paths.push_back(vec8);
	paths.push_back(vec9);
	paths.push_back(vec10);
	paths.push_back(vec11);
	paths.push_back(vec12);
	paths.push_back(vec13);
}

vector<std::vector<int>> Map::getPaths()
{
	return paths;
}

void Map::addTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, textureID);
	tile.addGroup(Game::groupMap);
}