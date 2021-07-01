#pragma once
#include <string>
#include <vector>
#include "Vector2D.h"

class Map
{
public:
	Map(std::string tID, int ms, int ts);
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xpos, int ypos);
	void init();
	std::vector<Vector2D*> getPoints() {return points;}
	std::vector<std::vector<int>> getPaths() {return paths;}
private:
	std::vector<std::vector<int>> paths;
	std::vector<Vector2D*> points;
	std::string textureID;
	int mapScale;
	int tileSize;
	int scaledSize;
};