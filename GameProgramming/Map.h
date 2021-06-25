#pragma once
#include <string>

class Map
{
public:
	Map(std::string tID, int ms, int ts);
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xpos, int ypos);
private:
	std::string textureID;
	int mapScale;
	int tileSize;
	int scaledSize;
};