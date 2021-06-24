#pragma once
#include <string>

class Map
{
public:
	Map(const char* mfp, int ms, int ts);
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xpos, int ypos);
private:
	const char* mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};